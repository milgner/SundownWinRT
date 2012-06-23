#include <ppl.h>
#include <ppltasks.h>

#include "markdown.h"
#include "html.h"
#include "buffer.h"

#include "SundownWinRT.h"

#define OUTPUT_UNIT 64

using namespace Tanoku;
using namespace Platform;

using Windows::Foundation::IAsyncOperation;
using Windows::ApplicationModel::PackageVersion;

static std::string PlatformToStdString(Platform::String^ platformString) {
	DWORD dwNum = WideCharToMultiByte(
		CP_UTF8, 
		NULL,
		platformString->Data(), 
		-1, 
		NULL, 
		0, 
		NULL, 
		NULL);
	if (dwNum == 0) {
		HRESULT res = HRESULT_FROM_WIN32(GetLastError());
		throw ref new Platform::COMException(res);
	}
	char* multiByteUtf8Text = new char[dwNum];
	WideCharToMultiByte(
		CP_UTF8, 
		NULL, 
		platformString->Data(), 
		-1, 
		multiByteUtf8Text, 
		dwNum, 
		NULL, 
		NULL
		);
	std::string result = multiByteUtf8Text;
	delete[] multiByteUtf8Text;
	return result;
}

static Platform::String^ StdToPlatformString(std::string& stdString) {
	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, stdString.data(), -1, NULL, 0);
	PWSTR wideText = new WCHAR[dwNum];
	MultiByteToWideChar(CP_UTF8, 0, stdString.data(), -1, wideText, dwNum);
	Platform::String^ result = ref new String(wideText);
	delete[] wideText;
	return result;
}

IAsyncOperation<String^>^ Sundown::renderToHtmlAsync(String^ markdownText) {
	return concurrency::create_async([markdownText]() -> String^ {
		std::string utf8Text = PlatformToStdString(markdownText);

		/* performing markdown parsing */
		auto ob = bufnew(OUTPUT_UNIT);

		sd_callbacks callbacks;
		html_renderopt options;
		sdhtml_renderer(&callbacks, &options, 0);
		
		sd_markdown* markdown = sd_markdown_new(0, 16, &callbacks, &options);
		
		sd_markdown_render(ob, (const uint8_t*)utf8Text.data(), utf8Text.length(), markdown);
		sd_markdown_free(markdown);

		std::string outputText((const char*)ob->data, ob->size);
		String^ result = StdToPlatformString(outputText);
		bufrelease(ob);
		return result;
	});
}

PackageVersion Sundown::getVersion() {
	int major, minor, revision;
	sd_version(&major, &minor, &revision);
	static PackageVersion result;
	result.Major = major;
	result.Minor = minor;
	result.Revision = revision;
	return result;
}
