// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InputBuffer.h"




void FARPG_InputBuffer::InputPressed(EARPG_InputType InputType)
{
	Data |= (1 << (uint8)InputType);
}

void FARPG_InputBuffer::InputReleased(EARPG_InputType InputType)
{
	Data &= ~(1 << (uint8)InputType);
}

bool FARPG_InputBuffer::IsPressed(int32 InputType) const
{
	return (Data & InputType) == InputType;
}

bool FARPG_InputBuffer::IsReleased(int32 InputType) const
{
	return !IsPressed(InputType);
}

bool FARPG_InputBuffer::IsAnyPressed(int32 InputType) const
{
	return Data & InputType;
}

bool FARPG_InputBuffer::IsAllReleased(int32 InputType) const
{
	return (Data & (~InputType)) == 0;
}
