//
// Created by Janam Shrestha on 10/2/25.
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <memory>
#include <vector>

#include "Window.h"
#include "Layer.h"


namespace Core {
    struct ApplicationSpecification {
        std::string Name = "Application";
        WindowSpecification WindowSpec;

    };
    class Application {
    public:
        explicit Application(const ApplicationSpecification& specification  = ApplicationSpecification());
        ~Application();

        void Run();
        void Stop();

        template<typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
        void PushLayer() {
            m_LayerStack.push_back(std::make_unique<TLayer>());
        }

        glm::vec2 GetFramebufferSize() const;
        static Application& Get();
        static float GetTime();

    private:
        ApplicationSpecification m_Specification;
        std::shared_ptr<Window> m_Window;

        bool m_Running = false;
        std::vector<std::unique_ptr<Layer>> m_LayerStack;



    };

}




#endif //APPLICATION_H
