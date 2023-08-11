import 'dart:convert';
import 'dart:async';
import 'package:gauges/gauges.dart';

import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:helanasa_app/models/helanasa_update.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Helanasa',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(title: 'HelaNASA'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  late ThermalisUpdate thermalis;

  Future<http.Response> fetchAPI() async {
    return await http.get(Uri.parse('http://192.168.0.100/get'));
  }

  // Future<http.Response> _forceTimeSpan() async {
  //   return await http.get(Uri.parse('http://192.168.0.100/set/force/2'));
  // }

  Future<ThermalisUpdate> processAPI() async {
    Future<ThermalisUpdate> result =
        Future<ThermalisUpdate>.value(ThermalisUpdate());
    try {
      final response = await fetchAPI();
      if (response.statusCode == 200) {
        return ThermalisUpdate.fromJson(jsonDecode(response.body));
      }
    } catch (e) {
      print(e.toString());
    }

    return result;
  }

  // void forceTimeSpan() async {
  //   await _forceTimeSpan();
  // }

  void _fetchAPI() async {
    thermalis = await processAPI();
    setState(() {});
  }

  late final Timer timer;

  @override
  void initState() {
    super.initState();
    thermalis = ThermalisUpdate();
    timer = Timer.periodic(const Duration(seconds: 2), (timer) {
      _fetchAPI();
    });
  }

  @override
  void dispose() {
    timer.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    TextStyle? setProfileStyle() {
      if (thermalis.tsForced == -1) {
        return Theme.of(context).textTheme.headlineSmall;
      } else {
        return Theme.of(context)
            .textTheme
            .headlineSmall
            ?.copyWith(color: Colors.red);
      }
    }

    Widget engineStatus() {
      if (thermalis.motorOn) {
        return Text(
          'PRENDIDA',
          textScaleFactor: 1.5,
          style: Theme.of(context)
              .textTheme
              .bodyLarge
              ?.copyWith(color: Colors.green),
        );
      } else {
        return Text(
          'APAGADA',
          textScaleFactor: 1.5,
          style: Theme.of(context)
              .textTheme
              .bodyLarge
              ?.copyWith(color: Colors.red),
        );
      }
    }

    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            const Text('Heladera', textScaleFactor: 1.5),
            engineStatus(),
            Container(
              margin: const EdgeInsets.only(bottom: 10),
            ),
            Stack(alignment: Alignment.bottomCenter, children: [
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: RadialGauge(axes: [
                  RadialGaugeAxis(
                      minValue: 0,
                      maxValue: 11,
                      minAngle: -130,
                      maxAngle: 130,
                      segments: [
                        RadialGaugeSegment(
                            minValue: 0,
                            maxValue: 4,
                            minAngle: -130,
                            maxAngle: -40 - 1,
                            color: Colors.blue[800]),
                        RadialGaugeSegment(
                            minValue: 4,
                            maxValue: 7,
                            minAngle: -40,
                            maxAngle: 40 - 1,
                            color: Colors.green[400]),
                        RadialGaugeSegment(
                            minValue: 7,
                            maxValue: 11,
                            minAngle: 40,
                            maxAngle: 130 - 1,
                            color: Colors.red[600]),
                      ],
                      pointers: [
                        RadialNeedlePointer(
                          value: thermalis.currentTemp,
                          thicknessStart: 20,
                          thicknessEnd: 0,
                          length: 0.9,
                          knobRadiusAbsolute: 10,
                          gradient: LinearGradient(
                            colors: [
                              Color(Colors.grey[400]!.value),
                              Color(Colors.grey[400]!.value),
                              Color(Colors.grey[600]!.value),
                              Color(Colors.grey[600]!.value)
                            ],
                            stops: const [0, 0.5, 0.5, 1],
                            begin: Alignment.topCenter,
                            end: Alignment.bottomCenter,
                          ),
                        )
                      ]),
                ]),
              ),
              Container(
                margin: const EdgeInsets.only(bottom: 25),
                child: Column(
                  children: [
                    Text(
                      thermalis.currentTemp.toStringAsFixed(1),
                      style: Theme.of(context).textTheme.headline2,
                    ),
                    Text(
                      thermalis.timeSpanName,
                      style: setProfileStyle(),
                    ),
                    Text('(${thermalis.tempMin}ºC a ${thermalis.tempMax}ºC)')
                  ],
                ),
              )
            ]),
            // ElevatedButton(
            //   onPressed: thermalis.tsForced == -1 ? forceTimeSpan : null,
            //   child: thermalis.tsForced == -1
            //       ? const Text('Forzar agua caliente')
            //       : const Text('Forzado temperatura máxima'),
            // )
          ],
        ),
      ),
      // floatingActionButton: FloatingActionButton(
      //   onPressed: _fetchAPI,
      //   tooltip: 'Increment',
      //   child: const Icon(Icons.add),
      // ),
    );
  }
}
