import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

ApplicationWindow {
    visible: true
    width: 600
    height: 600
    title: qsTr("Hello World")

    signal mouseClicked(double x, double y)
    signal updated(double x, double y)

    Item {
        anchors.fill: parent

        property bool collide: false

        focus: true
        Keys.onSpacePressed: {
            collide = !collide
        }

        MouseArea {
            anchors.fill: parent
            onMouseXChanged: if(worm.active) updated(mouse.x, mouse.y)
            onMouseYChanged: if(worm.active) updated(mouse.x, mouse.y)
            onClicked: mouseClicked(mouse.x, mouse.y)
        }

        Connections {
            target: fish
            onCoordsChanged: canvas.requestPaint()
        }

        Canvas {
            id: canvas
            anchors.fill: parent



            onPaint: {
                var fcoords = fish.coords
                var wcoords = worm.coords
                var ctx = getContext("2d")

                ctx.clearRect(0,0,600,600)
                ctx.fillStyle = '#42c1ff'
                ctx.fillRect(0,0,600,600)

                ctx.save()
                ctx.translate(fish.x, fish.y)
                ctx.rotate(fish.rotationAngle)
                //           ctx.drawImage('fishforward.png',  -25, -30)
                if( Math.abs(fish.absoleteAngle) <= Math.PI/2 )
                    ctx.drawImage('fishforward.png',  -65, -60)
                if( Math.abs(fish.absoleteAngle) > Math.PI/2 )
                    ctx.drawImage('fishbackward.png',  -50, -60)
                ctx.restore()


                if(parent.collide)
                {
                    ctx.save()
                    ctx.strokeStyle = 'red'
                    //create control points
                    ctx.fillStyle = 'red'
                    ctx.fillRect(fish.x + 2.5,fish.y + 2.5, -5, -5)
                    ctx.fillRect(fish.destX + 2.5,fish.destY + 2.5, -5, -5)
                    // create a triangle
                    ctx.beginPath()
                    ctx.moveTo(fcoords[3],fcoords[4])
                    ctx.lineTo(fcoords[0],fcoords[1])
                    ctx.lineTo(fcoords[6],fcoords[7])
                    ctx.closePath()
                    ctx.stroke()
                    ctx.restore()
                }


                if(worm.active)
                {
                    ctx.save()
                    ctx.translate(worm.x, worm.y)
                    ctx.rotate(worm.rotationAngle)
                    //           ctx.drawImage('fishforward.png',  -25, -30)
                    if( Math.abs(worm.absoleteAngle) <= Math.PI/2 )
                        ctx.drawImage('wormforward.png',  -20, -20)
                    if( Math.abs(worm.absoleteAngle) > Math.PI/2 )
                        ctx.drawImage('wormbackward.png',  -20, -20)
                    ctx.restore()

                    if(parent.collide)
                    {
                        ctx.save()
                        ctx.strokeStyle = 'red'
                        //create control points
                        ctx.fillStyle = 'red'
                        ctx.fillRect(worm.x + 2.5,worm.y + 2.5, -5, -5)
                        ctx.fillRect(worm.destX + 2.5,worm.destY + 2.5, -5, -5)
                        // create a triangle
                        ctx.beginPath()
                        ctx.moveTo(wcoords[3],wcoords[4])
                        ctx.lineTo(wcoords[0],wcoords[1])
                        ctx.lineTo(wcoords[6],wcoords[7])
                        ctx.closePath()
                        ctx.stroke()
                        ctx.restore()
                    }
                }

                if(booster.active)
                {
                    ctx.save()
                    ctx.translate(booster.x, booster.y)
                    ctx.drawImage('booster.png',  -20, -20)
                    ctx.restore()

                    if(parent.collide)
                    {
                        ctx.save()
                        ctx.fillStyle = 'blue'
                        ctx.fillRect(booster.x + 2.5,booster.y + 2.5, -5, -5)
                        ctx.restore()
                    }
                }
            }

            Component.onCompleted: {
                loadImage("fishforward.png")
                loadImage("fishbackward.png")
                loadImage("wormforward.png")
                loadImage("wormbackward.png")
                loadImage("booster.png")
            }
        }

        Item {
            id: infoBox

            width: 100
            height: 150

            anchors.margins: 5

            anchors.top: parent.top
            anchors.right: parent.right

            Rectangle {
                anchors.fill: parent

                color: "black"
                opacity: 0.7

                radius: 5

                layer.enabled: true
                layer.effect: Glow {
                    radius: 8
                    samples: 16
                    color: white
                }

            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 3

                Text {
                    text: qsTr("Best Points: %1").arg(game.bestpoints)
                    color: "white"
                }

                Text {
                    text: qsTr("Points: %1").arg(game.points)
                    color: "white"
                }

                Item { Layout.fillHeight: true }

                Text {
                    text: qsTr("Boost: %1").arg(worm.boost)
                    color: "white"
                }

                Text {
                    text: qsTr("Time left: %1").arg(worm.time)
                    color: "white"
                }

                Item { Layout.fillHeight: true }

                Text {
                    text: qsTr("Deaths: %1").arg(game.deaths);
                    color: "white"
                }

            }


        }
    }
}
