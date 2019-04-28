#ifndef Font_TTF_h__
#define Font_TTF_h__
#pragma once

#include <SDL_ttf.h>

class FontTTF
{
public:
	FontTTF();
	~FontTTF();

	bool LoadFont(const char* file, int ptsize, SDL_Color textColor = { 0x00, 0x00, 0x00, 0xFF });

	void Release();

	void ChangeSize(int ptsize) { m_ptsize = ptsize; }

	TTF_Font* GetFontPtr() const noexcept { return m_pFont; }
	const SDL_Color& GetColor() const noexcept { return m_textColor; }

private:
	TTF_Font* m_pFont = nullptr;
	int m_ptsize = 0;
	SDL_Color m_textColor = { 0x00, 0x00, 0x00, 0x00 };
};

#endif // Font_TTF_h__
