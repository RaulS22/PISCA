const express = require('express');
const router = express.Router();

// Armazenamento temporário na memória
let dadosSensores = [];

// Rota para receber dados do ESP32
router.post('/dados', (req, res) => {
    const { temperatura, umidade, pressao, altitude } = req.body;

if (
  temperatura === undefined ||
  umidade === undefined ||
  pressao === undefined ||
  altitude === undefined
) {

        return res.status(400).json({ erro: 'Dados incompletos' });
    }

    const registro = {
        temperatura,
        umidade,
        pressao,
        altitude,
        timestamp: new Date()
    };

    dadosSensores.push(registro);

    console.log("Novo dado recebido:", registro);

    res.json({ status: "OK", recebido: registro });
});

// Rota para visualizar dados armazenados
router.get('/dados', (req, res) => {
    res.json(dadosSensores);
});

module.exports = router;
