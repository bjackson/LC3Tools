//	Copyright 2003 Ashley Wise
//	University Of Illinois Urbana-Champaign
//	awise@crhc.uiuc.edu

#include "LC3bFileWindow.h"
#include "MainWindow.h"

using namespace std;
using namespace JMT;
using namespace LC3b;

namespace AshIDE	{

/*
FL_WHITE
FL_BLACK
FL_CYAN
FL_DARK_CYAN
FL_BLUE
FL_DARK_BLUE
FL_GREEN
FL_DARK_GREEN
FL_YELLOW
FL_DARK_YELLOW
FL_MAGENTA
FL_DARK_MAGENTA
FL_RED
FL_DARK_RED
enum TokenEnum {TUnknown, TBad, TComment, TCharConst, TCharacter, TString, TInteger, TReal, TIdentifier, TOperator, TDirective, TAttribute, TExpand, TData, TISA, TOpcode, TRegister};
*/
Fl_Text_Display::Style_Table_Entry LC3bFileWindow::StyleTable[] = 
	{	// Style table
		{ FL_BLACK,			FL_COURIER,			14 },	// A - Unknown
		{ FL_BLACK,			FL_COURIER,			14 },	// B - Bad
		{ FL_DARK_GREEN,	FL_COURIER,			14 },	// C - Comment
		{ FL_BLACK,			FL_COURIER,			14 },	// D - CharConst
		{ FL_MAGENTA,		FL_COURIER,			14 },	// E - Character
		{ FL_DARK_MAGENTA,	FL_COURIER,			14 },	// F - String
		{ FL_RED,			FL_COURIER,			14 },	// G - Integer
		{ FL_RED,			FL_COURIER_ITALIC,	14 },	// H - Real
		{ FL_BLACK,			FL_COURIER,			14 },	// I - Identifier
		{ FL_BLUE,			FL_COURIER,			14 },	// J - Operator
		{ FL_BLUE,			FL_COURIER,			14 },	// K - Directive
		{ FL_DARK_CYAN,		FL_COURIER_ITALIC,	14 },	// L - Attribute
		{ FL_BLACK,			FL_COURIER,			14 },	// M - Expand
		{ FL_DARK_CYAN,		FL_COURIER_BOLD,	14 },	// N - Data
		{ FL_BLACK,			FL_COURIER,			14 },	// O - ISA
		{ FL_DARK_BLUE,		FL_COURIER_BOLD,	14 },	// P - Opcode
		{ FL_DARK_RED,		FL_COURIER_BOLD,	14 }	// Q - Register
	};

list<Token *> LC3bFileWindow::TheList;
LC3bISA::Lexer LC3bFileWindow::TheLexer(LC3bFileWindow::TheList, MainWindow::MessageCallBack);
HighlightLexer LC3bFileWindow::TheHighlightLexer(&LC3bFileWindow::TheLexer, false);

LC3bFileWindow::LC3bFileWindow(int Width, int Height, const string &sFN) : FileWindow(Width, Height, sFN, StyleTable, sizeof(StyleTable) / sizeof((StyleTable)[0]))
{
	UpdateStyle(0, pTextEditor->pTextBuffer->length(), pTextEditor->pTextBuffer->length(), 0, NULL);
}

LC3bFileWindow::~LC3bFileWindow()
{
}

bool LC3bFileWindow::GetIdentifier(string &sIdentifier) const
{
	if(pTextEditor->pStyleBuffer->character(pTextEditor->insert_position()) == 'I')
	{
		char *sID = pTextEditor->pTextBuffer->text_range(pTextEditor->pTextBuffer->word_start(pTextEditor->insert_position()), pTextEditor->pTextBuffer->word_end(pTextEditor->insert_position()));
		sIdentifier = sID;
		delete sID;
		if(sIdentifier == "")
			return false;
		return true;
	}
	return false;
}

bool LC3bFileWindow::GetRegister(string &sRegister) const
{
	if(pTextEditor->pStyleBuffer->character(pTextEditor->insert_position()) == 'Q')
	{
		char *sID = pTextEditor->pTextBuffer->text_range(pTextEditor->pTextBuffer->word_start(pTextEditor->insert_position()), pTextEditor->pTextBuffer->word_end(pTextEditor->insert_position()));
		sRegister = sID;
		delete sID;
		if(sRegister == "")
			return false;
		return true;
	}
	return false;
}

bool LC3bFileWindow::Help() const
{
	static string sIdentifier;
	if(!FileWindow::GetIdentifier(sIdentifier))
		return false;
	if(pTextEditor->pStyleBuffer->character(pTextEditor->insert_position()) == 'P')
		return TheMainWindow.Help(LC3bInstructionIndex, sIdentifier);
	else
		return TheMainWindow.Help(AssemblerIndex, sIdentifier);
}

bool LC3bFileWindow::ParseStyle(const string &sText, string &sStyle)
{
	return TheHighlightLexer.Lex(sText, sStyle);
}

}	//namespace AshIDE
