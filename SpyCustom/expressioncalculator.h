#ifndef MATHLIB_EXPRESSION_CALCULATOR_H
#define MATHLIB_EXPRESSION_CALCULATOR_H
#ifdef _WIN32
#pragma once
#endif

#include "utlstring.h"
#include "utlstack.h"
#include "utlvector.h"


class CExpressionCalculator
{
public:
	CExpressionCalculator(const char* expr = NULL) : m_expr(expr) {}

	CExpressionCalculator(const CExpressionCalculator& x);
	CExpressionCalculator& operator=(const CExpressionCalculator& x);

public:
	void SetExpression(const char* expr)
	{
		m_expr = expr;
	}

	void SetVariable(const char* var, float value);
	void SetVariable(int nVariableIndex, float value);
	void ModifyVariable(const char* var, float value);

	int FindVariableIndex(const char* var);

	bool Evaluate(float& value);

	bool BuildVariableListFromExpression();

	int VariableCount();
	const char* VariableName(int nIndex);

private:
	bool ParseExpr(const char*& expr);
	bool ParseConditional(const char*& expr);
	bool ParseOr(const char*& expr);
	bool ParseAnd(const char*& expr);
	bool ParseEquality(const char*& expr);
	bool ParseLessGreater(const char*& expr);
	bool ParseAddSub(const char*& expr);
	bool ParseDivMul(const char*& expr);
	bool ParseUnary(const char*& expr);
	bool ParsePrimary(const char*& expr);
	bool Parse1ArgFunc(const char*& expr);
	bool Parse2ArgFunc(const char*& expr);
	bool Parse3ArgFunc(const char*& expr);
	bool Parse5ArgFunc(const char*& expr);

	CUtlString m_expr;
	CUtlVector< CUtlString > m_varNames;
	CUtlVector<float> m_varValues;
	CUtlStack<float> m_stack;
	bool m_bIsBuildingArgumentList;
};

float EvaluateExpression(char const* pExprString, float flValueToReturnIfFailure);


#endif  