// Fill out your copyright notice in the Description page of Project Settings.

#include "PoKeIt.h"
#include "Neuron.h"

Neuron::Neuron()
	: weight(0.0f), axiompotencial(0.0f)
{
}

Neuron::~Neuron()
{
}

float Neuron::getAxiompotencial()
{
	return axiompotencial;
}

void Neuron::setInputA(float input)
{
	inputA = input;
}

void Neuron::setInputB(float input)
{
	inputB = input;
}

void Neuron::setInputC(float input)
{
	inputC = input;
}