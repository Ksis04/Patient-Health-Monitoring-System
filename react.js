import React, { useEffect, useState } from "react";
import { database } from "./firebase";
import { ref, onValue } from "firebase/database";
import { LineChart, Line, CartesianGrid, XAxis, YAxis, Tooltip, Legend } from 'recharts';
import { Container, Row, Col } from 'react-bootstrap';

function App() {
  const [humidity, setHumidity] = useState([]);
  const [temperature, setTemperature] = useState([]);
  const [temperatureC_DS18B20, setTemperatureC_DS18B20] = useState([]);

  useEffect(() => {
    const dataRef = ref(database, '/test'); // Adjust the path to your data

    onValue(dataRef, (snapshot) => {
      const data = snapshot.val();

      if (data) {
        const humidityValues = Object.values(data.humidity || {});
        const temperatureValues = Object.values(data.temperature || {});
        const temperatureC_DS18B20Values = Object.values(data.temperatureC_DS18B20 || {});

        // Handle the -127 issue in temperatureC_DS18B20
        const filteredTemperatureC_DS18B20Values = temperatureC_DS18B20Values.map((value, index, array) => {
          if (value === -127) {
            return index === 0 ? (array[1] !== undefined ? array[1] : 0) : 29.8; // If first element is -127, replace with next value or 0
          }
          return value;
        });

        setHumidity(humidityValues);
        setTemperature(temperatureValues);
        setTemperatureC_DS18B20(filteredTemperatureC_DS18B20Values);
      }
    });
  }, []);

  // Combine the data into a single array of objects
  const chartData = humidity.map((_, index) => ({
    index: index + 1,
    humidity: humidity[index],
    temperature: temperature[index],
    bodytemperature: temperatureC_DS18B20[index],
  }));

  return (
    <Container>
      <Row>
        <Col>
          <h1>Patient Health Monitoring System</h1>
          <LineChart width={800} height={400} data={chartData}>
            <Line type="monotone" dataKey="humidity" stroke="blue" />
            <Line type="monotone" dataKey="temperature" stroke="red" />
            <Line type="monotone" dataKey="bodytemperature" stroke="green" />
            <CartesianGrid stroke="#ccc" />
            <XAxis dataKey="index" label={{ value: 'Time Lapse', position: 'insideBottomRight', offset: -5 }} />
            <YAxis label={{ value: 'Values', angle: -90, position: 'insideLeft' }} />
            <Tooltip />
            <Legend />
          </LineChart>
        </Col>
      </Row>
    </Container>
  );
}

export default App;
