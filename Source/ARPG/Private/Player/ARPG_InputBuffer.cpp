// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InputBuffer.h"



constexpr EARPG_InputType ARPG_InputType::GetMirroredAttackType(EARPG_InputType InputType)
{
	switch (InputType)
	{
	case EARPG_InputType::LeftLightAttack:
		return EARPG_InputType::RightLightAttack;
	case EARPG_InputType::RightLightAttack:
		return EARPG_InputType::LeftLightAttack;
	case EARPG_InputType::LeftHeavyAttack:
		return EARPG_InputType::RightHeavyAttack;
	case EARPG_InputType::RightHeavyAttack:
		return EARPG_InputType::LeftHeavyAttack;
	}
	check(0);
	return EARPG_InputType::Sprint;
}

constexpr int32 ARPG_InputType::MirrorAttackBitMask(int32 InputType)
{
	InputType = SwapBitMask(InputType, (uint8)EARPG_InputType::LeftLightAttack, (uint8)EARPG_InputType::RightLightAttack);
	InputType = SwapBitMask(InputType, (uint8)EARPG_InputType::LeftHeavyAttack, (uint8)EARPG_InputType::RightHeavyAttack);
	return InputType;
}

constexpr int32 ARPG_InputType::SwapBitMask(int32 BitMask, uint8 FirstIndexToSwap, uint8 SecondIndexToSwap)
{
	int32 Temporary = ((BitMask >> FirstIndexToSwap) ^ (BitMask >> SecondIndexToSwap)) & ((1U << 1) - 1); // XOR temporary
	return BitMask ^ ((Temporary << FirstIndexToSwap) | (Temporary << SecondIndexToSwap));
}

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
