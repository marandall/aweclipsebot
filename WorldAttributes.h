#pragma once
#include "awsdk_world.h"

class WorldAttributes : public AW::World::Attributes
{
public: // umbra override
	bool doUmbraFunction(Umbra::Variant& V, Umbra::FunctionRequest& R)
	{
		//
		//
		//

		UMBRA_FUNCTION_BEGIN(loadTemplate) {
			// required arguments
			UMBRA_FUNCTION_DA(0, File,    Umbra::VT_STRING);

			// code information break
			UMBRA_FUNCTION_CS() {
				// import the data
				AW::EnhancedINI ini;
				if (ini.Load(vaFile.toString().c_str()) == false)
				{
					UMBRA_FUNCTION_RET( false );
				}

				// import the enhanced INI
				setEnhancedINI(ini);
				UMBRA_FUNCTION_RET( true );
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// Interpolates between 2 sets of values
		//

		UMBRA_FUNCTION_BEGIN(interpolateBetween) {
			// required arguments
			UMBRA_FUNCTION_DA(0, Old, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(1, New, Umbra::VT_CLASS);
			UMBRA_FUNCTION_DA(2, Pro, Umbra::VT_FLOAT);

			UMBRA_FUNCTION_CC(Old, WorldAttributes);
			UMBRA_FUNCTION_CC(New, WorldAttributes);

			// code information break
			UMBRA_FUNCTION_CS() {
				interpolate(*pOld, *pNew, vaPro.toFloat());
			} UMBRA_FUNCTION_CE();
		} UMBRA_FUNCTION_END();

		// update
		return R.NOT_HANDLED;
	}

	int loadTemplate(const char* File)
	{
		// open an enhanced template
		AW::EnhancedINI ini;
		if (ini.Load(File) == false)
			return 1;

		// import the enhanced INI
		setEnhancedINI(ini);
		return 0;
	}
};