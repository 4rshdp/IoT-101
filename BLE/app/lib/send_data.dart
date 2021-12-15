import 'dart:async';
import 'dart:convert';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter_blue/flutter_blue.dart';



final String serviceUUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
//final String RXUUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
//final String TXUUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";



class SendData extends StatefulWidget{

  final BluetoothDevice device;
  SendData({Key? key, required this.device}) : super(key: key);

  SendDataState createState() => SendDataState();


}

class SendDataState extends State<SendData>{



  late BluetoothCharacteristic rX;
  String deviceValue = "";

  get gettingLoc => null;


  @override
  void dispose(){
    super.dispose();
  }
  @override
  void initState(){
    widget.device.connect().then((value) => discoverServices());

    super.initState();

  }
  void discoverServices() async{
    widget.device.discoverServices()
        .then((value) => value.forEach((element) {
      if(element.uuid.toString().contains(serviceUUID.toLowerCase())){
        element.characteristics.forEach((characteristic) {
          if(characteristic.properties.write | characteristic.properties.writeWithoutResponse){
            rX = characteristic;
          }
        });
      }
    }));

  }


  Future sendToNode(String msg) async
  {
    //print(msg);
    await rX.write(utf8.encode(msg));
  }





  setData()  async {
    print("Set Data method Called");

    print ("${NameController.text}");
    var nodeData = "${NameController.text}";
    await sendToNode(nodeData);
  }


  TextEditingController NameController = TextEditingController();



  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.device.name),
        actions: <Widget>[
          StreamBuilder<BluetoothDeviceState>(
            stream: widget.device.state,
            initialData: BluetoothDeviceState.connecting,
            builder: (c, snapshot) {
              VoidCallback? onPressed;
              String text;
              switch (snapshot.data) {
                case BluetoothDeviceState.connected:
                  onPressed = () => widget.device.disconnect();
                  text = 'DISCONNECT';
                  break;
                case BluetoothDeviceState.disconnected:
                  onPressed = () => widget.device.connect();
                  text = 'CONNECT';
                  break;
                default:
                  onPressed = null;
                  text = snapshot.data.toString().substring(21).toUpperCase();
                  break;
              }
              return TextButton(
                  onPressed: onPressed,
                  child: Text(
                    text,
                    style: Theme
                        .of(context)
                        .primaryTextTheme
                        .button
                        ?.copyWith(color: Colors.white),
                  )
              );
            },
          )
        ],
      ),
      body: Container(
          child: Column(
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.all(16),
                child: TextField(
                  controller:NameController,
                  decoration: InputDecoration(labelText: 'data'),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16),
                child: ElevatedButton(
                  onPressed: () async{
                    await setData();
                  },


                  child: Text('send msg'),
                ),
              )
            ],
          )
      ),
    );
  }
}


