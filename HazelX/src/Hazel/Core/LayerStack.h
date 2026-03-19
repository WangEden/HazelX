#pragma once

#include "Hazel/Core/Base.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
		// std::vector<Layer*>::iterator m_LayerInsert; // 一个迭代器
		// ::iterator begin()/end() 让这个类型支持迭代操作，即：
		/*
			for (Layer* layer : m_LayerStack) 
			{
				layer->OnUpdate();
			}
		*/
	};

}
