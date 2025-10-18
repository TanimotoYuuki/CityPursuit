#include "stdafx.h"
#include "QteEventInput.h"
#include "QteEvent.h"

bool QteEventInput::Start()
{
	m_qteEvent = FindGO<QteEvent>("qteevent");
	return true;
}

//実行処理
void QteEventInput::Execute()
{
	//コマンドリストの取得
	auto commandList = m_qteEvent->GetInputCommand();
	
	//コマンド入力が失敗していない かつ
	//最後のコマンド入力していなければ
	//コマンド入力することができる
	if (!m_isInputFailed && !m_isInputLastCommand)
	{
		//コマンド入力
		InputCommand();
	}

	//コマンド入力が一致していたら
	if (commandList[m_nowInputCommandOrder] == m_inputCommand)
	{
		m_qteEvent->SuccessInputCommand(m_inputCommand);//コマンド入力成功を通知する

		//次のコマンドがあるかどうか
		if (m_nowInputCommandOrder < commandList.size() - 1)
		{
			m_nowInputCommandOrder++;//次のコマンドの順番へ
		}
		//次のコマンドがなければ
		else
		{
			m_isInputLastCommand = true;//最後のコマンド入力できている
		}
	}
	//コマンド入力が一致していなければ
	else
	{
		if (m_inputCommand != -1)
		{
			m_isInputFailed = true;//コマンド入力失敗している
		}
	}
}

//コマンド入力
void QteEventInput::InputCommand()
{
	m_inputCommand = -1;

	//左上方向
	if (g_pad[0]->IsTriggerLStickLeftUp()) { m_inputCommand = QteEvent::enGamePadInputList_LeftUpArrow; }
	//左下方向
	else if (g_pad[0]->IsTriggerLStickLeftDown()) { m_inputCommand = QteEvent::enGamePadInputList_LeftDownArrow; }
	//右上方向
	else if (g_pad[0]->IsTriggerLStickRightUp()) { m_inputCommand = QteEvent::enGamePadInputList_RightUpArrow; }
	//右下方向
	else if (g_pad[0]->IsTriggerLStickRightDown()) { m_inputCommand = QteEvent::enGamePadInputList_RightDownArrow; }

	//上方向
	else if (g_pad[0]->IsTriggerLStickUp()) { m_inputCommand = QteEvent::enGamePadInputList_UpArrow; }
	//下方向
	else if (g_pad[0]->IsTriggerLStickDown()) { m_inputCommand = QteEvent::enGamePadInputList_DownArrow; }
	//左方向
	else if (g_pad[0]->IsTriggerLStickLeft()) { m_inputCommand = QteEvent::enGamePadInputList_LeftArrow; }
	//右方向
	else if (g_pad[0]->IsTriggerLStickRight()) { m_inputCommand = QteEvent::enGamePadInputList_RightArrow; }

	//Aボタン
	else if (g_pad[0]->IsTrigger(enButtonA)) { m_inputCommand = QteEvent::enGamePadInputList_AButton; }
	//Bボタン
	else if (g_pad[0]->IsTrigger(enButtonB)) { m_inputCommand = QteEvent::enGamePadInputList_BButton; }
	//Xボタン
	else if (g_pad[0]->IsTrigger(enButtonX)) { m_inputCommand = QteEvent::enGamePadInputList_XButton; }
	//Yボタン
	else if (g_pad[0]->IsTrigger(enButtonY)) { m_inputCommand = QteEvent::enGamePadInputList_YButton; }
}