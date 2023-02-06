#pragma once
#include "Element.h"
#include <unordered_map>
#include <vector>

class Grid : public Element
{
public:

	Grid(const Grid&&) = delete;
	Grid(const Grid&) = delete;
	Grid(int x, int y, int width, int height, unsigned char align);

	// Inherited via Element
	virtual void Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget) override;
	virtual void SetOpacity(float opacity) override;
	virtual float GetOpacity() override;
	virtual void Move(int x, int y) override;
	virtual HWND Show(HWND hParent, HINSTANCE hInstance) override;
	virtual void OnPaint(ID2D1RenderTarget* pRenderTarget) override;


	void DefineRow(int height);
	
	template<class I, class ...Args>
	void DefineRow(I height, Args... args);

	void DefineColumn(int width);

	template<class I, class ...Args>
	void DefineColumn(I width, Args... args);

	void AddElement(int row, int column, Element& rElement);

private:

	std::vector<int> columns;
	std::vector<int> rows;
	std::unordered_map<int, Element*> elements;
	

};

template<class I, class ...Args>
inline void Grid::DefineRow(I height, Args ...args)
{
	if (sizeof...(args) == 0)
	{
		DefineRow(height);
		return;
	}
	rows.push_back(height);
	DefineRow(args...);
}

template<class I, class ...Args>
inline void Grid::DefineColumn(I width, Args ...args)
{
	if (sizeof...(args) == 0)
	{
		DefineColumn(width);
		return;
	}
	columns.push_back(width);
	DefineColumn(args...);
}
