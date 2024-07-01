import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.15
import org.kde.plasma.components 3.0 as PlasmaComponents
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import QtQuick.Window 2.2

Item{



    Plasmoid.fullRepresentation: Item
    {


    ColumnLayout{

        id: coolers
        anchors.left: parent.left
        anchors.right: parent.right

        PlasmaComponents.Label
        {
            property string display: if (slider1.value <= 30) {return "Auto"} else return slider1.value;
            id: label1
            text: "CPU Cooler: " + display;
        }

        PlasmaComponents.Slider
        {
            id: slider1;
            implicitWidth: parent.width
            from: 30
            to: 100;
            stepSize: 5;
        }

        PlasmaComponents.Label
        {
            property string display: if (slider2.value <= 30) {return "Auto"} else return slider2.value;
            id: label2
            text: "GPU Cooler: " + display;
        }

        PlasmaComponents.Slider
        {
            id: slider2
            from: 30
            to: 100;
            stepSize: 5;
            implicitWidth: parent.width;

        }

        //TODO Remove since plasma has proper power management applet now
        ColumnLayout{
            id: radioGroup
            anchors.top: slider2.bottom

            Repeater{
                id: governorRepeater

            model: Plasmoid.nativeInterface.governors;

            PlasmaComponents.RadioButton
                {
                    id: radioButton
                    text: modelData

                    checked: text == Plasmoid.nativeInterface.governor;

                    onCheckedChanged: {
                        if (checked) {
                            Plasmoid.nativeInterface.setGovernor(radioButton.text);
                        }
                    }
                }
            }

            //When plasmoid is active
            //check if current governor
            //corresponds to the actual governor in the system
            Timer {
                property string governorRN;

                interval: 2000 // 2 seconds
                running: Plasmoid.expanded
                repeat: true

                onTriggered: {
                    governorRN = Plasmoid.nativeInterface.governor;
                    // Check if the current radioButton matches the current governor value
                    for (var i = 0; i < governorRepeater.count; ++i) {

                        var radioButton = governorRepeater.itemAt(i);
                        radioButton.checked = (radioButton.text == governorRN);
                    }
                }
            }
        }


        Item{
            anchors.top: radioGroup.bottom
            PlasmaComponents.Label {
                id: powerLabel
                text: Plasmoid.nativeInterface.nvidiaPower
            }
        }
        Timer
        {
            id:timer
            interval: 2300
            repeat: true
            running: Plasmoid.expanded
            onTriggered:
            {
                Plasmoid.nativeInterface.gpuCool = slider2.value;
                Plasmoid.nativeInterface.cpuCool = slider1.value;
                powerLabel.text = Plasmoid.nativeInterface.nvidiaPower;
            }
        }


        Plasmoid.onExpandedChanged:
        {
            if(!Plasmoid.expanded)
            {
                Plasmoid.nativeInterface.gpuCool = slider2.value;
                Plasmoid.nativeInterface.cpuCool = slider1.value;
            }
        }
    }

    }

    Plasmoid.compactRepresentation: Item
    {
        id: compRoot

        MouseArea {
                id: mouseArea

                property bool wasExpanded: false

                anchors.fill: parent
                hoverEnabled: false
                onPressed: wasExpanded = Plasmoid.expanded
                onClicked: {

                    Plasmoid.expanded = !wasExpanded
                }
        }

        Rectangle{

            anchors.centerIn: parent
            height: parent.height*0.5
            width: height

            border.color: PlasmaCore.Theme.textColor
            border.width: 2

            radius: 1
            color: Plasmoid.expanded ? PlasmaCore.Theme.highlightColor : "transparent"
            rotation: 45
        }

    }
}
