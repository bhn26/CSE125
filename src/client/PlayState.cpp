#include "PlayState.h"
#include "StateManager.h"
#include "MenuState.h"
#include <sstream>

using namespace std;

CPlayState::CPlayState(CStateManager* pManager)
 : CGameState(pManager), m_pFont(NULL), m_pScoreControl(NULL), 
  m_ulCurrentScore(0), m_bGameOver(false)
{
	AddFontResource((LPCWSTR)"01 Digitall.ttf");
	
	m_pFont = new CGameFont;
	m_pFont->CreateFont("01 Digitall", 20, FW_NORMAL);

	m_pScoreControl = new CTextControl(m_pFont,TRectanglei(145,210,620,730));
	m_pScoreControl->SetAlignement(CTextControl::TACenter);
	m_pScoreControl->SetTextColor(1.0f,0.588f,0.039f);

	m_pBackgroundImg = CImage::CreateImage("PlayBckgnd.png",TRectanglei(0,600,0,800));
}

CPlayState::~CPlayState()
{
	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
	RemoveFontResource((LPCWSTR)"01 Digitall.ttf");
}

CPlayState* CPlayState::GetInstance(CStateManager* pManager)
{
	static CPlayState Instance(pManager);
	return &Instance;
}

void CPlayState::Reset()
{
	m_ulCurrentScore = 0;
	m_bGameOver = false;
}

void CPlayState::OnKeyDown(WPARAM wKey)
{
	switch (wKey)
	{
	case VK_UP:
		if (!m_bGameOver)
			//m_pMatrix->ShapeRotate();
		break;
	case VK_DOWN:
		if (!m_bGameOver)	
			//m_pMatrix->ShapeDown();
		break;
	case VK_LEFT:
		if (!m_bGameOver)
			//m_pMatrix->ShapeLeft();
		break;
	case VK_RIGHT:
		if (!m_bGameOver)
			//m_pMatrix->ShapeRight();
		break;
	case VK_ESCAPE:
		ChangeState(CMenuState::GetInstance(m_pStateManager));
		break;
	case VK_RETURN:
		if (m_bGameOver)
		{
			
		}
	}
}

void CPlayState::Update(DWORD dwCurrentTime)
{
	if (!m_bGameOver)
	{
	}
}

void CPlayState::Draw()  
{ 
	m_pBackgroundImg->BlitImage(0,0);


	stringstream ssScore;
	ssScore << m_ulCurrentScore;
	m_pScoreControl->SetText(ssScore.str());
	m_pScoreControl->Draw();

	if (m_bGameOver)
	{
		// In game over, we draw a semi-transparent black screen on top
		// of the background. This is possible because blending has 
		// been enabled.
		glColor4f(0.0,0.0,0.0,0.5);
		// Disable 2D texturing because we want to draw a non
		// textured rectangle over the screen.
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glVertex3i(0,0,0);
		glVertex3i(0,600,0);
		glVertex3i(800,600,0);
		glVertex3i(800,0,0);
		glEnd();
		glEnable(GL_TEXTURE_2D);

		m_pFont->DrawText("GAME OVER",340,200);
		m_pFont->DrawText("Press Enter to continue",285,300);
	}

}

