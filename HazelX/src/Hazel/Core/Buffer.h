#pragma once

#include "Hazel/Core/Base.h"

namespace Hazel {

	struct Buffer
	{

		byte* Data;
		uint32_t Size;

		Buffer()
			: Data(nullptr), Size(0)
		{
		}

		Buffer(byte* data, uint32_t size)
			: Data(data), Size(size)
		{
		}

		static Buffer Copy(void* data, uint32_t size) // 深拷贝
		{
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}

		void Allocate(uint32_t size)
		{
			delete[] Data;
			Data = nullptr;

			if (size == 0)
				return;

			Data = new byte[size]; // 申请一块堆内存
			Size = size;
		}

		void ZeroInitialize()
		{
			if (Data)
				memset(Data, 0, Size);
		}

		void Write(void* data, uint32_t size, uint32_t offset = 0) // 在offset处写入数据
		{
			HZ_CORE_ASSERT(offset + size <= Size, "Buffer overflow!");
			memcpy(Data + offset, data, size);
		}

		operator bool() const // 用于重载需要对该类进行逻辑判断的地方
		{
			return Data;
		}

		byte& operator[](int index)
		{
			return Data[index];
		}

		// 当该类型作为const类型对象时可调用这个，
		// 因为const对象不能调用非const函数，
		// 没有这个函数，const类型的该对象中[]运算符就无法工作；
		byte operator[](int index) const 
		{
			return Data[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		inline uint32_t GetSize() const { return Size; }
	};
}
