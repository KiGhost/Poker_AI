// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class POKEIT_API Neuron
{
public:
	Neuron();
	~Neuron();

	float getAxiompotencial();
	void setInputA(float input);
	void setInputB(float input);
	void setInputC(float input);

private:
	float weight, axiompotencial, inputA, inputB, inputC;
	bool firing;
};