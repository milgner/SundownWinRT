#pragma once

namespace Tanoku {
	public ref class Sundown sealed {
	public:
		/************************************************************************/
		/* Asynchronously render the given Markdown to HTML                     */
		/************************************************************************/
		static Windows::Foundation::IAsyncOperation<Platform::String^>^
			renderToHtmlAsync(Platform::String^);

		/************************************************************************/
		/* Get the version of the Sundown library                               */
		/************************************************************************/
		static Windows::ApplicationModel::PackageVersion getVersion();

	};
}