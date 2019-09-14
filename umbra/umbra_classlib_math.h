#pragma once
#include "umbra_classlib.h"

// open namespace of Math
namespace Umbra { namespace ClassLib {

	class MathDef : public Umbra::RemoteClass
	{
	public: // static constants
		static const double E;       // = 2.71828182845904523536;
		static const double LOG2E;   // = 1.44269504088896340736;
		static const double LOG10E;  // = 0.434294481903251827651;
		static const double LN2;     // = 0.693147180559945309417;
		static const double LN10;    // = 2.30258509299404568402;
		static const double PI;      // = 3.14159265358979323846;
		static const double PI_2;    // = 1.57079632679489661923;
		static const double PI_4;    // = 0.785398163397448309616;
		static const double SQRT2;   // = 1.41421356237309504880;
		static const double SQRT1_2; // = 0.707106781186547524401

	public: // handling methods
		bool doUmbraProperty(Variant& V, PropertyRequest& R)
		{
			// mathematical constants provided by the CRT
			UMBRA_PROPERTY_RR(E);
			UMBRA_PROPERTY_RR(LOG2E);
			UMBRA_PROPERTY_RR(LOG10E);
			UMBRA_PROPERTY_RR(LN2);
			UMBRA_PROPERTY_RR(LN10);
			UMBRA_PROPERTY_RR(PI);
			UMBRA_PROPERTY_RR(PI_2);
			UMBRA_PROPERTY_RR(PI_4);
			UMBRA_PROPERTY_RR(SQRT2);
			UMBRA_PROPERTY_RR(SQRT1_2);

			// unable to handle request
			return R.NOT_HANDLED;
		}

		bool doUmbraFunction(Variant& V, FunctionRequest& R)
		{
			// trig sin function
			UMBRA_FUNCTION_BEGIN(sin)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::sin(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// trig cos function
			UMBRA_FUNCTION_BEGIN(cos)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::cos(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// trig tan function
			UMBRA_FUNCTION_BEGIN(tan)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::tan(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			//
			// Hyperbole Trig
			//

			// trig sin function
			UMBRA_FUNCTION_BEGIN(sinh)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::sinh(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// trig cos function
			UMBRA_FUNCTION_BEGIN(cosh)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::cosh(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// trig tan function
			UMBRA_FUNCTION_BEGIN(tanh)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::tanh(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			//
			// Arc Trig
			//

			// trig sin function
			UMBRA_FUNCTION_BEGIN(asin)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::asin(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// trig cos function
			UMBRA_FUNCTION_BEGIN(acos)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::acos(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// trig tan function
			UMBRA_FUNCTION_BEGIN(atan)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::atan(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// trig tan function
			UMBRA_FUNCTION_BEGIN(atan2)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				UMBRA_FUNCTION_DA(1, Val2, Umbra::VT_NUMBER)
				// code block
				UMBRA_FUNCTION_CS() {;
					UMBRA_FUNCTION_RET( ::atan2(vaVal.toDouble(), vaVal2.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			//
			// Absolute Functions
			//

			// absolute function for integers
			UMBRA_FUNCTION_BEGIN(abs)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::abs(vaVal.toInt32()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// absolute function for floating point
			UMBRA_FUNCTION_BEGIN(abs)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::fabs(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// moderrus
			UMBRA_FUNCTION_BEGIN(mod)
				UMBRA_FUNCTION_DA(0, A, Umbra::VT_NUMBER);
				UMBRA_FUNCTION_DA(1, B, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( vaA.toInt64() % vaB.toInt64() );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// 
			// Logarithms
			//

			// logarithm naturale
			UMBRA_FUNCTION_BEGIN(log)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::log(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// logarithm naturale
			UMBRA_FUNCTION_BEGIN(log10)
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::log10(vaVal.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			//
			// Indices
			//

			// power expression
			UMBRA_FUNCTION_BEGIN(pow)
				UMBRA_FUNCTION_DA(0, X, Umbra::VT_NUMBER);
				UMBRA_FUNCTION_DA(1, Y, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::pow(vaX.toDouble(), vaY.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// square root
			UMBRA_FUNCTION_BEGIN(sqr)
				UMBRA_FUNCTION_DA(0, X, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::pow(vaX.toDouble(), 0.5) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			// root of any value
			UMBRA_FUNCTION_BEGIN(root)
				UMBRA_FUNCTION_DA(0, X, Umbra::VT_NUMBER);
				UMBRA_FUNCTION_DA(1, Y, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ::pow(vaX.toDouble(), 1.0 / vaY.toDouble()) );
				} UMBRA_FUNCTION_CE()
			UMBRA_FUNCTION_END();

			//
			// Converting between angles
			//

			// convert degrees to radians
			UMBRA_FUNCTION_BEGIN(degToRad) {
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ((PI * 2) / 360.0) * vaVal.toDouble() );
				} UMBRA_FUNCTION_CE()
			} UMBRA_FUNCTION_END();

			// convert radians to degrees
			UMBRA_FUNCTION_BEGIN(radToDeg) {
				UMBRA_FUNCTION_DA(0, Val, Umbra::VT_NUMBER);
				// code block
				UMBRA_FUNCTION_CS() {
					UMBRA_FUNCTION_RET( ((PI * 2) / 360.0) / vaVal.toDouble() );
				} UMBRA_FUNCTION_CE()
			} UMBRA_FUNCTION_END();

			//
			// Random Number Access
			//

			// convert radians to degrees
			UMBRA_FUNCTION_BEGIN(rand) {
				// randomise the function
				srand((unsigned int)time(0));

				// code block
				UMBRA_FUNCTION_CS() {
					// generate 64 bit number
					__int64 tool = 0;
					for (size_t i = 0; i < 8; i++)
						tool = (tool << 8) | (rand() % 0xff);

					// result is known
					UMBRA_FUNCTION_RET( (int)tool );
				} UMBRA_FUNCTION_CE()
			} UMBRA_FUNCTION_END();

			// convert radians to degrees
			UMBRA_FUNCTION_BEGIN(random) {
				UMBRA_FUNCTION_DA(0, Low,  Umbra::VT_NUMBER);
				UMBRA_FUNCTION_DA(1, High, Umbra::VT_NUMBER);

				// code block
				UMBRA_FUNCTION_CS() {
					// randomise the function
					srand((unsigned int)time(0));

					// generate 64 bit number
					__int64 tool = 0;
					for (size_t i = 0; i < 8; i++)
						tool = (tool << 8) | (rand() % 0xff);

					// convert into relevant portions
					__int64 iHigh	= vaHigh.toInt64();
					__int64 iLow	= vaLow.toInt64();

					// map as moderus
					__int64 range	= abs((int)tool) % (iHigh - iLow);
					__int64 result	= range + iLow;

					// h = 10
					// l = -5
					//
					// (h - l) = 15
					// 15 + -5 = 10;
					//

					// result is known
					UMBRA_FUNCTION_RET( result );
				} UMBRA_FUNCTION_CE()
			} UMBRA_FUNCTION_END();

			// unable to handle request
			return R.NOT_HANDLED;
		}

	public: // static const
		static MathDef& getInst()
		{
			static MathDef d;
			return d;
		}
	};

// end of namespace Math
} }