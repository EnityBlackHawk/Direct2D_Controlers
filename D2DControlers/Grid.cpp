#include "Grid.h"

Grid::Grid(int x, int y, int width, int height, unsigned char align):
    Element(x, y, width, height, align)
{
}

void Grid::Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget)
{


    for (auto e : elements)
    {
        int row = e.first / 10;
        int column = e.first - (row * 10);

        int x = 1;
        for (int i = row - 1; i > -1; i--)
            x += rows[i];

        int y = 1;
        for (int i = column - 1; i > -1; i--)
            x += columns[i];
      
        e.second->SetPos(x, y);
        e.second->Create(NULL, NULL, pRenderTarget);
    }

    
}

void Grid::SetOpacity(float opacity)
{
}

void Grid::Move(int x, int y)
{
}

HWND Grid::Show(HWND hParent, HINSTANCE hInstance)
{
    return HWND();
}

void Grid::OnPaint(ID2D1RenderTarget* pRenderTarget)
{
    for (auto e : elements)
    {
        e.second->OnPaint(pRenderTarget);
    }
}

void Grid::DefineRow(int height)
{
    rows.push_back(height);
}

void Grid::DefineColumn(int width)
{
    columns.push_back(width);
}

void Grid::AddElement(int row, int column, Element& rElement)
{
    elements[row * 10 + column] = &rElement;
}
