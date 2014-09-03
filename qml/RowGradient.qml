import QtQuick 2.0
import "theme.js" as Theme

Rectangle {
    id: root

    gradient: Gradient {
        GradientStop { position: 0; color: Theme.colors.viewBackgroundHigh }
        GradientStop { position: 1 - 1 / root.height ; color: Theme.colors.viewBackgroundLow; }
        GradientStop { position: 1; color: Theme.colors.viewBackgroundBottom }
    }
}
