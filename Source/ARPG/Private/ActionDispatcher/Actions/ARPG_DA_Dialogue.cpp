// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_DA_Dialogue.h"
#include "CharacterBase.h"

TSet<AActor*> UARPG_DA_SpeakOneSentence::GetAllRegistableEntities() const
{
	return { Speaker.Get() };
}

void UARPG_DA_SpeakOneSentence::WhenActionActived()
{
	Speaker->Speak(Sentence, FOnCharacterBehaviorFinished::CreateWeakLambda(this, [=](bool Succeed)
		{
			if (Succeed)
			{
				ExecuteEventAndFinishAction(WhenSpeakFinished);
			}
			else
			{
				AbortDispatcher();
			}
		}));
}

void UARPG_DA_SpeakOneSentence::WhenActionDeactived()
{
	Speaker->StopSpeak();
}
