#pragma once

// convert from one string to return the same string
String convertTypeToString(String Type);
String convertTypeToString(int Type);
String convertTypeToString(float Type);
String convertTypeToString(AW::Colour Type);

void convertStringToType(String Source, String& Type);
void convertStringToType(String Source, float& Type);
void convertStringToType(String Source, int& Type);
void convertStringToType(String Source, AW::Colour& Type);