#pragma once
enum class CullOption
{
	CULL_FRONT,
	CULL_BACK,
	CULL_OFF
};
enum class ZTestOption
{
	Z_TEST_ON,
	Z_TEST_OFF
};
enum class ZTestCompareOption
{
	LESS,
	LESS_EQUAL,
	EQUAL,
	GREAT_EQUAL,
	GREAT
};

enum class ZWriteOption
{
	Z_WRITE_ON,
	Z_WRITE_OFF
};
enum class AlphaTestOption
{
	ALPHA_TEST_ON,
	ALPHA_TEST_OFF
};
enum class ZCalculateOption
{
	AUTO,
	MANUAL
};

class ShaderOption
{
public:
	CullOption cullOption;
	ZTestOption zTestOption;
	ZTestCompareOption zTestCompareOption;
	ZWriteOption zWriteOption;
	AlphaTestOption alphaTestOption;
	ZCalculateOption zCalculateOption;
	ShaderOption()
	{
		cullOption = CullOption::CULL_BACK;
		zTestOption = ZTestOption::Z_TEST_ON;
		zTestCompareOption = ZTestCompareOption::LESS;
		zWriteOption = ZWriteOption::Z_WRITE_ON;
		alphaTestOption = AlphaTestOption::ALPHA_TEST_OFF;
		zCalculateOption = ZCalculateOption::AUTO;
	}
};