#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer();
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void Allocate(unsigned int count);

	inline unsigned int getCount() const{ return m_Count; };
};