#pragma once

#include "common/Blob.hpp"
#include "SNES/MMU.hpp"

#pragma warning(push, 0)
#include <cctype>
#include <fstream>
#include <memory>
#pragma warning(pop)

namespace MXNES {
namespace SNES {

/**
 * Class which encapsulates an SNES ROM
 */
class ROM final: public Component, private Dependency<Core> {
MXNES_DECLARE_TESTRUNNER_ACCESS;
public:

	ROM();
	~ROM() = default;
	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(ROM);

	bool open_file(const std::string filePath);

	void reset();
	std::string to_string() const;
	void from_string(const std::string source);

private:
	static constexpr u8 _HEADERSIZ = 0x40;
	typedef UTIL::Blob<u8, _HEADERSIZ> Header;

	struct Info {
		Info();
		
		const std::string to_string() const;

		//True if the ROM has an additional SMC header
		bool hasAdditionalHeader;
		MMU::MappingModel mappingModel;

		//The file size
		u64 size;
	} _info;

	/**
	 * @brief Searches the ROM for a valid header.
	 *
	 *	While SNES ROM headers are always loaded at 0xFFC0, the actual address of the
	 *	header in the ROM varies with the mapping model. We locate the header with
	 *	a fair degree of certainty by searching for an uppercase ASCII string, padded
	 *  with spaces, which describes the ROM name.
	 *
	 * @param fileStream The current file stream
	 * @return A pointer to a Blob containing the loaded header
	 */
	Header* _find_valid_header(std::ifstream &fileStream) const;
	bool _check_header(std::ifstream &fileStream, const u32 romAddr) const;

};

} //namespace SNES
} //namespace MXNES