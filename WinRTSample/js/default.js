// For an introduction to the Blank template, see the following documentation:
// http://go.microsoft.com/fwlink/?LinkId=232509
(function () {
    "use strict";

    var app = WinJS.Application;
    var activation = Windows.ApplicationModel.Activation;
    WinJS.strictProcessing();

    var loadMarkdown = function () {
        var uri = new Windows.Foundation.Uri("ms-appx:///README.markdown");
        Windows.Storage.StorageFile.getFileFromApplicationUriAsync(uri).then(function (storageFile) {
            return Windows.Storage.FileIO.readTextAsync(storageFile);
        }).then(function (content) {
            return Tanoku.Sundown.renderToHtmlAsync(content);
        }).then(function (markdown) {
            var container = document.getElementById("container");
            WinJS.Utilities.setInnerHTMLUnsafe(container, markdown);
        });
    };

    app.onactivated = function (args) {
        if (args.detail.kind === activation.ActivationKind.launch) {
            if (args.detail.previousExecutionState !== activation.ApplicationExecutionState.terminated) {
                loadMarkdown();
            }
            args.setPromise(WinJS.UI.processAll());
        }
    };

    app.start();
})();
