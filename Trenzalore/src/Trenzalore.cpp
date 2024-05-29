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
		Sphere* sphere1 = new Sphere();
		sphere1->m_Radius = 0.1f;

		Sphere* sphere2 = new Sphere();
		sphere2->m_Color = { 0.0f, 1.0f, 1.0f };
		sphere2->m_Radius = 0.1f;
		sphere2->m_Position = { 1.0f, 0.0f, -5.0f };
		m_Scene.Add(sphere2);
		m_Scene.Add(sphere1);
	}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.Update(ts);
		
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render Time: %0.3fms", m_RenderTime);
		if(ImGui::Button("Render")) 
		{
			Render();
		}
		ImGui::End();

		ImGui::Begin("Scene Options");
		ImGui::DragFloat3("Position", glm::value_ptr(m_Scene.m_Objects[0]->m_Position), 0.1f);
		//ImGui::DragFloat("Radius", &m_Scene.m_Objects[0].Radius, 0.1f);
		ImGui::ColorPicker3("Color", glm::value_ptr(m_Scene.m_Objects[0]->m_Color));
		ImGui::End();

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