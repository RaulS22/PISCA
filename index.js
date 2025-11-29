const express = require('express');
const app = express();
const sensorRoutes = require('./routes/sensor');

app.use(express.json());

// Rotas
app.use('/sensor', sensorRoutes);

// Rota base
app.get('/', (req, res) => {
    res.send('Servidor Express funcionando');
});

const port = process.env.PORT || 3000;
app.listen(port, () => {
    console.log(`Servidor rodando em http://localhost:${port}`);
});


// Para testar sem o ESP32 rodar no bash:

// curl -X POST http://localhost:3000/sensor/dados \
// -H "Content-Type: application/json" \
// -d '{"temperatura":25.5,"umidade":60.2,"pressao":957.9,"altitude":642.0}'
