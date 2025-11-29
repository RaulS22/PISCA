const express = require('express');
const app = express();
const sensorRoutes = require('./routes/sensor');

// Para aceitar JSON vindo do ESP32
app.use(express.json());

// Rotas
app.use('/sensor', sensorRoutes);

// Rota base
app.get('/', (req, res) => {
    res.send('Servidor Express funcionando');
});

const port = process.env.PORT || 3000;

app.listen(port, () => {
    console.log(`Servidor rodando em http://IP:${port}`);
});
