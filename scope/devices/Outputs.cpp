#include "stdafx.h"
#include "Outputs.h"

namespace scope {

Outputs::Outputs(const uint32_t& _area)
	: area(_area)
	, writeabort(false) {

}

Outputs::~Outputs() {

}

void Outputs::AbortWrite() {
	writeabort = true;
}



}