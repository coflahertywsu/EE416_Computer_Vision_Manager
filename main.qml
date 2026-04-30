import QtQuick 2.4
import QtQuick.Window 2.1

Window {
    id: mainWindow
    visible: true
    visibility: Window.Maximized
    color: "#303030"

    Loader {
        id: pageLoader
        anchors.fill: parent
        source: "HomePage.qml"
    }

    onWidthChanged: {
        if (pageLoader.item)
            pageLoader.item.pageLoader = pageLoader
    }

    onHeightChanged: {
        if (pageLoader.item)
            pageLoader.item.pageLoader = pageLoader
    }

    Connections {
        target: pageLoader

        function onLoaded() {
            if (pageLoader.item)
                pageLoader.item.pageLoader = pageLoader
        }
    }
}
