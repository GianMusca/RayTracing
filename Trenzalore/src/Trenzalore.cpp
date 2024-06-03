#include "Renderer.h"
#include "Camera.h"

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "glm\gtc\type_ptr.hpp"


using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f)
	{
		Material& blueSphere = m_Scene.m_Materials.emplace_back();
		blueSphere.Albedo = glm::vec3(0.0f, 0.0f, 1.0f);
		blueSphere.Roughness = 0.5f;

		Material& redSphere = m_Scene.m_Materials.emplace_back();
		redSphere.Albedo = glm::vec3(1.0f, 0.0f, 0.0f);
		redSphere.Roughness = 0.5f;

		Sphere* sphere1 = new Sphere();
		sphere1->m_Position = { 0.0f, -101.0f, 0.0f };
		sphere1->m_Radius = 100.0f;
		sphere1->m_MaterialIndex = 0;

		Sphere* sphere2 = new Sphere();
		sphere2->m_Radius = 1.0f;
		sphere2->m_Position = { 0.0f, 0.0f, 0.0f };
		sphere2->m_MaterialIndex = 1;

		m_Scene.Add(sphere2);
		m_Scene.Add(sphere1);
	}

	virtual void OnUpdate(float ts) override
	{
		if (m_Camera.Update(ts))
		{
			m_Renderer.ResetFrameIndex();
		}
		
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render Time: %0.3fms", m_RenderTime);
		if(ImGui::Button("Render")) 
		{
			Render();
		}

		if (ImGui::Button("Reset"))
		{
			m_Renderer.ResetFrameIndex();
		}
		ImGui::End();

		ImGui::Begin("Scene Options");
		for(int i = 0; i < m_Scene.GetObjects().size(); i++)
		{
			Object* object = m_Scene.m_Objects[i];

			ImGui::PushID(i);
			ImGui::Text("Object %d", i);
			ImGui::DragFloat3("Position", glm::value_ptr(object->m_Position), 0.1f);
			//ImGui::DragFloat("Radius", &object->m_Radius, 0.1f);

			ImGui::ColorEdit3("Color", glm::value_ptr(m_Scene.m_Materials[object->m_MaterialIndex].Albedo));
			ImGui::DragFloat("Roughness", &(m_Scene.m_Materials[object->m_MaterialIndex].Roughness), 0.1f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &m_Scene.m_Materials[object->m_MaterialIndex].Metallic, 0.1f, 0.0f, 1.0f);

			ImGui::PopID();
		}
		ImGui::End();

		ImGui::ShowDemoWindow();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		auto image = m_Renderer.GetDisplayImage();
		if(image)
		{
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnCanvasResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnCanvasResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);

		m_RenderTime = timer.ElapsedMillis();
	}

	
private:
	Trenzalore::Renderer m_Renderer;
	Camera m_Camera;
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;
	float m_RenderTime = 0.0f;
	Scene m_Scene;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}