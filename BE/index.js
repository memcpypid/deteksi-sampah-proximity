// ============================
// IMPORT & KONFIGURASI
// ============================
const express = require("express");
const http = require("http");
const socketIo = require("socket.io");
const bodyParser = require("body-parser");
const cors = require("cors");
const { Sequelize, DataTypes } = require("sequelize");

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
  cors: {
    origin: "*",
    methods: ["GET", "POST", "PUT", "DELETE", "OPTIONS"],
  },
});

// Middleware
app.use(bodyParser.json());
app.use(cors());

// ============================
// DATABASE (SQLite) & MODEL
// ============================
const sequelize = new Sequelize({
  dialect: "sqlite",
  storage: "fuzzy_data.db",
  logging: false,
});

const FuzzySample = sequelize.define(
  "FuzzySample",
  {
    humidity: DataTypes.FLOAT,
    induktif: DataTypes.INTEGER,
    kapasitif: DataTypes.INTEGER,
    prediksi: DataTypes.STRING,
    aktual: DataTypes.STRING,

    tp_organik: DataTypes.INTEGER,
    tn_organik: DataTypes.INTEGER,
    fp_organik: DataTypes.INTEGER,
    fn_organik: DataTypes.INTEGER,
    precision_organik: DataTypes.FLOAT,
    recall_organik: DataTypes.FLOAT,
    accuracy_organik: DataTypes.FLOAT,

    tp_anorganik: DataTypes.INTEGER,
    tn_anorganik: DataTypes.INTEGER,
    fp_anorganik: DataTypes.INTEGER,
    fn_anorganik: DataTypes.INTEGER,
    precision_anorganik: DataTypes.FLOAT,
    recall_anorganik: DataTypes.FLOAT,
    accuracy_anorganik: DataTypes.FLOAT,

    accuracy_total: DataTypes.FLOAT,
  },
  {
    timestamps: true,
  }
);

// Sync database
sequelize.sync();

// ============================
// ENDPOINT TAMBAH DATA
// ============================
app.post("/api/sample", async (req, res) => {
  try {
    const sample = await FuzzySample.create(req.body);

    // Kirim data ke semua client via WebSocket
    io.emit("sensor_data_update", req.body);

    res.json({
      status: "ok",
      id: sample.id,
      timestamp: sample.createdAt,
    });
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: "Gagal menyimpan data" });
  }
});

// ============================
// ENDPOINT LIHAT SEMUA DATA
// ============================
app.get("/api/samples", async (req, res) => {
  try {
    const { from, to } = req.query;
    const where = {};
    if (from || to) {
      where.createdAt = {};
      if (from) where.createdAt[Sequelize.Op.gte] = new Date(from);
      if (to) where.createdAt[Sequelize.Op.lte] = new Date(to);
    }
    const samples = await FuzzySample.findAll({
      where,
      order: [["createdAt", "DESC"]],
    });
    res.json(samples);
  } catch (e) {
    console.error(e);
    res.status(400).json({ error: "Invalid date filter" });
  }
});

// ============================
// ENDPOINT HISTORY
// ============================
app.get("/api/history", async (req, res) => {
  try {
    const { from, to } = req.query;
    let where;
    if (from || to) {
      where = { createdAt: {} };
      if (from) where.createdAt[Sequelize.Op.gte] = new Date(from);
      if (to) where.createdAt[Sequelize.Op.lte] = new Date(to);
    } else {
      const since = new Date(Date.now() - 24 * 3600 * 1000);
      where = { createdAt: { [Sequelize.Op.gte]: since } };
    }
    const samples = await FuzzySample.findAll({
      where,
      order: [["createdAt", "DESC"]],
    });
    res.json(samples);
  } catch (e) {
    console.error(e);
    res.status(400).json({ error: "Invalid date filter" });
  }
});

// ============================
// ENDPOINT CLEAR HISTORY
// ============================
app.delete("/api/clear_history", async (req, res) => {
  await FuzzySample.destroy({ where: {} });
  io.emit("history_cleared", { message: "All data cleared" });
  res.json({ status: "cleared" });
});

// ============================
// WEBSOCKET EVENTS
// ============================
let isRecording = false;

io.on("connection", (socket) => {
  console.log("🔌 User Connected via WebSocket:", socket.id);

  socket.emit("connected", { message: "Welcome!", id: socket.id });
  socket.emit("record_status", { recording: isRecording });

  socket.broadcast.emit("user_connected", {
    message: `User ${socket.id} connected`,
    id: socket.id,
  });

  socket.on("selected_class", (data) => {
    console.log(" Kelas target dari frontend:", data.class);
    io.emit("selected_class_BE", { class: data.class, id: socket.id });
  });

  socket.on("reset", () => {
    console.log("Reset Metriks dari frontend");
    io.emit("reset_system", { id: socket.id });
  });

  socket.on("start_record", () => {
    isRecording = true;
    console.log("⏺ Start Recording by", socket.id);
    io.emit("record_status", { recording: true, by: socket.id });
  });

  socket.on("stop_record", () => {
    isRecording = false;
    console.log(" Stop Recording by", socket.id);
    io.emit("record_status", { recording: false, by: socket.id });
  });

  socket.on("sensor_data", (data) => {
    console.log(" Data dari client:", data);
    io.emit("sensor_data_update", data);
  });

  socket.on("disconnect", () => {
    console.log(" Client disconnected:", socket.id);
    io.emit("user_disconnected", {
      message: `User ${socket.id} disconnected`,
      id: socket.id,
    });
  });
});

// ============================
// RUN SERVER
// ============================
const PORT = process.env.PORT || 5000;
server.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
