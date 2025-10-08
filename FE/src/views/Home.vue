<template>
    <div class="min-h-screen bg-gradient-to-br from-slate-50 to-slate-100">
        <!-- Header -->
        <header class="bg-white shadow-lg border-b border-slate-200">
            <div class="container mx-auto px-6 py-4">
                <div class="flex items-center justify-between">
                    <div class="flex items-center space-x-4">
                        <div class="bg-gradient-to-br from-emerald-500 to-teal-600 p-3 rounded-xl shadow-lg">
                            <svg class="w-8 h-8 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                                    d="M19 11H5m14 0a2 2 0 012 2v6a2 2 0 01-2 2H5a2 2 0 01-2-2v-6a2 2 0 012-2m14 0V9a2 2 0 00-2-2M5 11V9a2 2 0 012-2m0 0V5a2 2 0 012-2h6a2 2 0 012 2v2M7 7h10" />
                            </svg>
                        </div>
                        <div>
                            <h1 class="text-2xl font-bold text-slate-800">Smart Waste Detection System</h1>
                            <p class="text-sm text-slate-500">Fuzzy Logic Based Classification</p>
                        </div>
                    </div>
                    <div class="flex items-center space-x-3">
                        <div class="text-right">
                            <p class="text-xs text-slate-500">Status Sistem</p>
                            <div class="flex items-center space-x-2">
                                <span class="flex h-3 w-3">
                                    <span
                                        class="animate-ping absolute inline-flex h-3 w-3 rounded-full bg-emerald-400 opacity-75"></span>
                                    <span class="relative inline-flex rounded-full h-3 w-3 bg-emerald-500"></span>
                                </span>
                                <span class="text-sm font-semibold text-emerald-600">Online</span>
                            </div>
                        </div>
                    </div>

                </div>

            </div>
        </header>

        <!-- Main Content -->
        <main class="container mx-auto px-6 py-8">
            <!-- Sensor Status Cards -->
            <div class="grid grid-cols-1 md:grid-cols-3 gap-6 mb-8">
                <!-- Inductive Sensor -->
                <div
                    class="bg-white rounded-2xl shadow-lg p-6 border border-slate-200 hover:shadow-xl transition-shadow">
                    <div class="flex items-center justify-between mb-4">
                        <h3 class="text-lg font-semibold text-slate-700">Sensor Inductive</h3>
                        <div class="bg-blue-100 p-2 rounded-lg">
                            <svg class="w-6 h-6 text-blue-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                                    d="M13 10V3L4 14h7v7l9-11h-7z" />
                            </svg>
                        </div>
                    </div>
                    <div class="space-y-2">
                        <div class="flex justify-between items-center">
                            <span class="text-sm text-slate-500">Status:</span>
                            <span
                                :class="sensorData.inductive.status === 'Detected' ? 'text-emerald-600' : 'text-slate-400'"
                                class="text-sm font-semibold">
                                {{ sensorData.inductive.status }}
                            </span>
                        </div>
                        <div class="flex justify-between items-center">
                            <span class="text-sm text-slate-500">Value:</span>
                            <span class="text-2xl font-bold text-slate-800">{{ sensorData.inductive.value }}</span>
                        </div>
                        <div class="w-full bg-slate-200 rounded-full h-2 mt-3">
                            <div class="bg-blue-500 h-2 rounded-full transition-all duration-500"
                                :style="{ width: sensorData.inductive.value + '%' }"></div>
                        </div>
                    </div>
                </div>

                <!-- Capacitive Sensor -->
                <div
                    class="bg-white rounded-2xl shadow-lg p-6 border border-slate-200 hover:shadow-xl transition-shadow">
                    <div class="flex items-center justify-between mb-4">
                        <h3 class="text-lg font-semibold text-slate-700">Sensor Capacitive</h3>
                        <div class="bg-purple-100 p-2 rounded-lg">
                            <svg class="w-6 h-6 text-purple-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                                    d="M7 21a4 4 0 01-4-4V5a2 2 0 012-2h4a2 2 0 012 2v12a4 4 0 01-4 4zm0 0h12a2 2 0 002-2v-4a2 2 0 00-2-2h-2.343M11 7.343l1.657-1.657a2 2 0 012.828 0l2.829 2.829a2 2 0 010 2.828l-8.486 8.485M7 17h.01" />
                            </svg>
                        </div>
                    </div>
                    <div class="space-y-2">
                        <div class="flex justify-between items-center">
                            <span class="text-sm text-slate-500">Status:</span>
                            <span
                                :class="sensorData.capacitive.status === 'Detected' ? 'text-emerald-600' : 'text-slate-400'"
                                class="text-sm font-semibold">
                                {{ sensorData.capacitive.status }}
                            </span>
                        </div>
                        <div class="flex justify-between items-center">
                            <span class="text-sm text-slate-500">Value:</span>
                            <span class="text-2xl font-bold text-slate-800">{{ sensorData.capacitive.value }}</span>
                        </div>
                        <div class="w-full bg-slate-200 rounded-full h-2 mt-3">
                            <div class="bg-purple-500 h-2 rounded-full transition-all duration-500"
                                :style="{ width: sensorData.capacitive.value + '%' }"></div>
                        </div>
                    </div>
                </div>

                <!-- DHT22 Sensor -->
                <div
                    class="bg-white rounded-2xl shadow-lg p-6 border border-slate-200 hover:shadow-xl transition-shadow">
                    <div class="flex items-center justify-between mb-4">
                        <h3 class="text-lg font-semibold text-slate-700">Sensor DHT22</h3>
                        <div class="bg-amber-100 p-2 rounded-lg">
                            <svg class="w-6 h-6 text-amber-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                                    d="M3 15a4 4 0 004 4h9a5 5 0 10-.1-9.999 5.002 5.002 0 10-9.78 2.096A4.001 4.001 0 003 15z" />
                            </svg>
                        </div>
                    </div>
                    <div class="grid grid-cols-2 gap-4">
                        <div class="text-center">
                            <p class="text-xs text-slate-500 mb-1">Temperature</p>
                            <p class="text-2xl font-bold text-slate-800">{{ sensorData.dht22.temperature }}°C</p>
                        </div>
                        <div class="text-center">
                            <p class="text-xs text-slate-500 mb-1">Humidity</p>
                            <p class="text-2xl font-bold text-slate-800">{{ sensorData.dht22.humidity }}%</p>
                        </div>
                    </div>
                </div>
            </div>
            <div
                class="flex items-center justify-center space-x-4 mb-6 bg-white rounded-2xl shadow-lg p-8  border border-slate-200">
                <label for="classSelect" class="text-sm font-medium text-slate-700">Pilih Jenis Sampah:</label>
                <select v-model="selectedClass" id="classSelect" class="border rounded-lg p-2">
                    <option disabled :value="null">-- Pilih --</option>
                    <option :value="0">ORGANIK</option>
                    <option :value="1">ANORGANIK</option>
                </select>

                <button @click="sendSelectedClass"
                    class="bg-emerald-600 text-white px-4 py-2 rounded-lg hover:bg-emerald-700 transition">
                    Kirim ke Sistem
                </button>
                <button @click="resetSistem"
                    class="bg-red-600 text-white px-4 py-2 rounded-lg hover:bg-red-700 transition">
                    Reset Sistem
                </button>

                <div class="w-px h-6 bg-slate-200 mx-2"></div>

                <div class="flex items-center gap-3">
                    <span v-if="recording" class="inline-flex items-center text-sm font-medium text-rose-600">
                        <span class="relative flex h-2.5 w-2.5 mr-2">
                            <span class="animate-ping absolute inline-flex h-full w-full rounded-full bg-rose-400 opacity-75"></span>
                            <span class="relative inline-flex rounded-full h-2.5 w-2.5 bg-rose-500"></span>
                        </span>
                        Merekam
                    </span>

                    <button @click="startRecord" :disabled="recording"
                        class="px-4 py-2 rounded-lg transition text-white disabled:opacity-50 disabled:cursor-not-allowed"
                        :class="recording ? 'bg-slate-400' : 'bg-indigo-600 hover:bg-indigo-700'">
                        Start Record
                    </button>
                    <button @click="stopRecord" :disabled="!recording"
                        class="px-4 py-2 rounded-lg transition text-white disabled:opacity-50 disabled:cursor-not-allowed"
                        :class="!recording ? 'bg-slate-400' : 'bg-amber-600 hover:bg-amber-700'">
                        Stop Record
                    </button>
                </div>
            </div>

            <!-- Fuzzy Logic Result -->
            <div class="bg-white rounded-2xl shadow-lg p-8 mb-8 border border-slate-200">
                <h2 class="text-2xl font-bold text-slate-800 mb-6 flex items-center">
                    <div class="bg-gradient-to-br from-emerald-500 to-teal-600 p-2 rounded-lg mr-3">
                        <svg class="w-6 h-6 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                                d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z" />
                        </svg>
                    </div>
                    Hasil Klasifikasi Fuzzy Logic
                </h2>
                <div class="grid grid-cols-1 md:grid-cols-2 gap-8">
                    <div
                        class="text-center p-8 bg-gradient-to-br from-emerald-50 to-teal-50 rounded-xl border-2 border-emerald-200">
                        <p class="text-sm font-semibold text-slate-600 mb-2">Jenis Sampah Terdeteksi</p>
                        <p :class="fuzzyResult.wasteType === 'ORGANIK' ? 'text-emerald-600' : 'text-blue-600'"
                            class="text-4xl font-bold mb-3">
                            {{ fuzzyResult.wasteType }}
                        </p>
                        <div class="flex items-center justify-center space-x-2">
                            <span class="text-sm text-slate-600">Confidence:</span>
                            <span class="text-xl font-bold text-slate-800">{{ fuzzyResult.confidence }}%</span>
                        </div>
                        <div class="mt-4 flex items-center justify-center gap-3">
                            <span class="text-xs font-semibold text-slate-600">Actual:</span>
                            <span :class="fuzzyResult.actualType === 'ORGANIK' ? 'text-emerald-600' : 'text-blue-600'" class="text-lg font-bold">
                                {{ fuzzyResult.actualType }}
                            </span>
                            <span :class="fuzzyResult.actualType && fuzzyResult.wasteType && fuzzyResult.actualType === fuzzyResult.wasteType ? 'bg-emerald-100 text-emerald-700' : 'bg-rose-100 text-rose-700'" class="px-2 py-0.5 rounded-full text-xs font-medium">
                                {{ fuzzyResult.actualType === fuzzyResult.wasteType ? 'Cocok' : 'Tidak Cocok' }}
                            </span>
                        </div>
                    </div>
                    <div class="space-y-4">
                        <div class="flex items-center justify-between p-4 bg-slate-50 rounded-lg">
                            <span class="text-sm font-medium text-slate-700">Fuzzy Input 1 (Proximity
                                Capacitive):</span>
                            <span class="text-lg font-bold text-slate-800">{{ fuzzyResult.input1 }}</span>
                        </div>
                        <div class="flex items-center justify-between p-4 bg-slate-50 rounded-lg">
                            <span class="text-sm font-medium text-slate-700">Fuzzy Input 2 (Proximity Inductive):</span>
                            <span class="text-lg font-bold text-slate-800">{{ fuzzyResult.input2 }}</span>
                        </div>
                        <div class="flex items-center justify-between p-4 bg-slate-50 rounded-lg">
                            <span class="text-sm font-medium text-slate-700">Fuzzy Input 3 (DHT 22):</span>
                            <span class="text-lg font-bold text-slate-800">{{ fuzzyResult.envFactor }}%</span>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Performance Metrics Charts -->
            <div class="bg-white rounded-2xl shadow-lg p-8 border border-slate-200">
                <h2 class="text-2xl font-bold text-slate-800 mb-6 flex items-center">
                    <div class="bg-gradient-to-br from-blue-500 to-indigo-600 p-2 rounded-lg mr-3">
                        <svg class="w-6 h-6 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                                d="M9 19v-6a2 2 0 00-2-2H5a2 2 0 00-2 2v6a2 2 0 002 2h2a2 2 0 002-2zm0 0V9a2 2 0 012-2h2a2 2 0 012 2v10m-6 0a2 2 0 002 2h2a2 2 0 002-2m0 0V5a2 2 0 012-2h2a2 2 0 012 2v14a2 2 0 01-2 2h-2a2 2 0 01-2-2z" />
                        </svg>
                    </div>
                    Metrik Performa Sistem
                </h2>

                <div class="grid grid-cols-1 md:grid-cols-3 gap-8">
                    <!-- Accuracy Chart -->
                    <div class="text-center">
                        <h3 class="text-lg font-semibold text-slate-700 mb-4">Akurasi</h3>
                        <div class="relative inline-flex items-center justify-center">
                            <svg class="transform -rotate-90 w-40 h-40">
                                <circle cx="80" cy="80" r="70" stroke="#e2e8f0" stroke-width="12" fill="none" />
                                <circle cx="80" cy="80" r="70" stroke="#10b981" stroke-width="12" fill="none"
                                    :stroke-dasharray="439.6"
                                    :stroke-dashoffset="439.6 - (439.6 * metrics.accuracy) / 100"
                                    class="transition-all duration-1000 ease-out" />
                            </svg>
                            <div class="absolute">
                                <p class="text-3xl font-bold text-slate-800">{{ metrics.accuracy }}%</p>
                            </div>
                        </div>
                        <p class="mt-4 text-sm text-slate-500">Tingkat keakuratan klasifikasi sampah</p>
                    </div>

                    <!-- Precision Chart -->
                    <div class="text-center">
                        <h3 class="text-lg font-semibold text-slate-700 mb-4">Precision</h3>
                        <div class="relative inline-flex items-center justify-center">
                            <svg class="transform -rotate-90 w-40 h-40">
                                <circle cx="80" cy="80" r="70" stroke="#e2e8f0" stroke-width="12" fill="none" />
                                <circle cx="80" cy="80" r="70" stroke="#3b82f6" stroke-width="12" fill="none"
                                    :stroke-dasharray="439.6"
                                    :stroke-dashoffset="439.6 - (439.6 * metrics.precision) / 100"
                                    class="transition-all duration-1000 ease-out" />
                            </svg>
                            <div class="absolute">
                                <p class="text-3xl font-bold text-slate-800">{{ metrics.precision }}%</p>
                            </div>
                        </div>
                        <p class="mt-4 text-sm text-slate-500">Ketepatan prediksi positif</p>
                    </div>

                    <!-- Recall Chart -->
                    <div class="text-center">
                        <h3 class="text-lg font-semibold text-slate-700 mb-4">Recall</h3>
                        <div class="relative inline-flex items-center justify-center">
                            <svg class="transform -rotate-90 w-40 h-40">
                                <circle cx="80" cy="80" r="70" stroke="#e2e8f0" stroke-width="12" fill="none" />
                                <circle cx="80" cy="80" r="70" stroke="#8b5cf6" stroke-width="12" fill="none"
                                    :stroke-dasharray="439.6"
                                    :stroke-dashoffset="439.6 - (439.6 * metrics.recall) / 100"
                                    class="transition-all duration-1000 ease-out" />
                            </svg>
                            <div class="absolute">
                                <p class="text-3xl font-bold text-slate-800">{{ metrics.recall }}%</p>
                            </div>
                        </div>
                        <p class="mt-4 text-sm text-slate-500">Kemampuan mendeteksi sampah yang benar</p>
                    </div>
                </div>

                <!-- Performance Summary -->
                <div class="mt-8 p-6 bg-gradient-to-r from-slate-50 to-slate-100 rounded-xl">
                    <div class="grid grid-cols-1 md:grid-cols-3 gap-4">
                        <div class="text-center">
                            <p class="text-xs text-slate-500 mb-1">Total Deteksi</p>
                            <p class="text-2xl font-bold text-slate-800">{{ performanceData.totalDetections }}</p>
                        </div>
                        <div class="text-center">
                            <p class="text-xs text-slate-500 mb-1">Organik Terdeteksi</p>
                            <p class="text-2xl font-bold text-emerald-600">{{ performanceData.organicCount }}</p>
                        </div>
                        <div class="text-center">
                            <p class="text-xs text-slate-500 mb-1">Anorganik Terdeteksi</p>
                            <p class="text-2xl font-bold text-blue-600">{{ performanceData.inorganicCount }}</p>
                        </div>
                    </div>
                </div>

            </div>

    
            <div class="mt-8">
                <PerformanceChart :performance="performanceByClass" />
            </div>

            <div class="mt-8">
                <ResultsTable />
            </div>
        </main>

        <!-- Footer -->
        <footer class="bg-white border-t border-slate-200 mt-12">
            <div class="container mx-auto px-6 py-8">
                <div class="grid grid-cols-1 md:grid-cols-3 gap-8">
                    <div>
                        <h3 class="text-lg font-bold text-slate-800 mb-3">Smart Waste Detection</h3>
                        <p class="text-sm text-slate-600">Sistem klasifikasi sampah cerdas menggunakan fuzzy logic untuk
                            memisahkan sampah organik dan anorganik secara otomatis.</p>
                    </div>
                    <div>
                        <h3 class="text-lg font-bold text-slate-800 mb-3">Teknologi</h3>
                        <ul class="space-y-2 text-sm text-slate-600">
                            <li class="flex items-center">
                                <span class="w-2 h-2 bg-emerald-500 rounded-full mr-2"></span>
                                Proximity Inductive Sensor
                            </li>
                            <li class="flex items-center">
                                <span class="w-2 h-2 bg-purple-500 rounded-full mr-2"></span>
                                Proximity Capacitive Sensor
                            </li>
                            <li class="flex items-center">
                                <span class="w-2 h-2 bg-amber-500 rounded-full mr-2"></span>
                                DHT22 Temperature & Humidity
                            </li>
                            <li class="flex items-center">
                                <span class="w-2 h-2 bg-blue-500 rounded-full mr-2"></span>
                                Fuzzy Logic Algorithm
                            </li>
                        </ul>
                    </div>
                    <div>
                        <h3 class="text-lg font-bold text-slate-800 mb-3">Informasi</h3>
                        <p class="text-xs text-slate-500">© 2025 Smart Waste Detection System. All rights reserved.</p>
                    </div>
                </div>
                <div class="border-t border-slate-200 mt-8 pt-6 text-center">
                    <p class="text-sm text-slate-500">Sistem monitoring real-time untuk deteksi dan klasifikasi sampah
                        berbasis IoT</p>
                </div>
            </div>
        </footer>
    </div>
</template>

<script>
import { socket } from '../services/websocket';
import PerformanceChart from '../components/PerformanceChart.vue';
import ResultsTable from '../components/ResultsTable.vue';

export default {
    name: 'WasteDetectionDashboard',
    components: { PerformanceChart, ResultsTable },
    data() {
        return {
            selectedClass: 0,
            sensorData: {
                inductive: { value: 0, status: 'Menunggu...' },
                capacitive: { value: 0, status: 'Menunggu...' },
                dht22: { temperature: 0, humidity: 0 }
            },
            fuzzyResult: {
                wasteType: '-',
                confidence: 0,
                input1: 0,
                input2: 0,
                envFactor: 0,
                actualType: '-'
            },
            metrics: { accuracy: 0, precision: 0, recall: 0 },
            performanceData: { totalDetections: 0, organicCount: 0, inorganicCount: 0 },
            performanceByClass: {
                precision_organik: 0,
                recall_organik: 0,
                accuracy_organik: 0,
                confidence_organik: 0,
                precision_anorganik: 0,
                recall_anorganik: 0,
                accuracy_anorganik: 0,
                confidence_anorganik: 0
            },
            recording: false,
            connected: false
        }
    },
    mounted() { this.setupSocket(); },
    methods: {
        toPercent(v) {
            if (v == null || isNaN(v)) return 0;
            return v > 1 ? v : v * 100;
        },
        resetSistem() {
            socket.emit("reset", {});
            console.log("🔄 Sistem direset");
        },
        startRecord() {
            socket.emit('start_record', { by: this.mySocketId });
        },
        stopRecord() {
            socket.emit('stop_record', { by: this.mySocketId });
        },
        sendSelectedClass() {
            if (this.selectedClass === null) { alert("Pilih jenis sampah terlebih dahulu!"); return; }
            socket.emit("selected_class", { class: this.selectedClass });
            console.log("📤 Kelas dikirim ke backend:", this.selectedClass);
        },
        setupSocket() {
            console.log("🔌 Menghubungkan ke Socket.IO server...");
            socket.on("connected", (data) => { this.connected = true; this.mySocketId = data.id; });
            socket.on("user_connected", (data) => { console.log("👤 User baru connect:", data); });
            socket.on("user_disconnected", (data) => { console.log("👤 User disconnected:", data); });
            socket.on("sensor_data_update", (payload) => { this.updateDashboard(payload); });
            socket.on("record_status", (st) => { this.recording = !!st.recording; });
            socket.on("disconnect", () => { this.connected = false; });
            socket.io.on("reconnect_attempt", () => { console.log("🔄 Mencoba reconnect..."); });
        },
        updateDashboard(payload) {
            // Sensor booleans -> 0/100 visual value
            this.sensorData.inductive.value = payload.induktif ? 100 : 0;
            this.sensorData.capacitive.value = payload.kapasitif ? 100 : 0;
            this.sensorData.dht22.humidity = payload.humidity || 0;
            this.sensorData.dht22.temperature = payload.temperature || 0;
            this.sensorData.inductive.status = payload.induktif ? 'Detected' : 'Tidak';
            this.sensorData.capacitive.status = payload.kapasitif ? 'Detected' : 'Tidak';

            // Fuzzy results and inputs (Input 1 = Capacitive, Input 2 = Inductive)
            this.fuzzyResult.wasteType = payload.prediksi || 'Tidak diketahui';
            this.fuzzyResult.actualType = payload.aktual || '-';
            this.fuzzyResult.confidence = parseFloat((this.toPercent(payload.confidence_total || 0)).toFixed(2));
            this.fuzzyResult.input1 = payload.kapasitif;
            this.fuzzyResult.input2 = payload.induktif;
            this.fuzzyResult.envFactor = payload.humidity || 0;

            // Overall metrics (0..1 -> 0..100)
            this.metrics.accuracy = parseFloat((this.toPercent(payload.accuracy_total || 0)).toFixed(2));
            const precisionAvg = ((payload.precision_organik || 0) + (payload.precision_anorganik || 0)) / 2;
            const recallAvg = ((payload.recall_organik || 0) + (payload.recall_anorganik || 0)) / 2;
            this.metrics.precision = parseFloat((this.toPercent(precisionAvg)).toFixed(2));
            this.metrics.recall = parseFloat((this.toPercent(recallAvg)).toFixed(2));

            // Counters
            this.performanceData.totalDetections = payload.total_prediction || 0;
            this.performanceData.organicCount = payload.organik_detect || 0;
            this.performanceData.inorganicCount = payload.anorganik_detect || 0;

            // Class-wise metrics (0..1 -> 0..100)
            this.performanceByClass.precision_organik = this.toPercent(payload.precision_organik || 0);
            this.performanceByClass.recall_organik = this.toPercent(payload.recall_organik || 0);
            this.performanceByClass.accuracy_organik = this.toPercent(payload.accuracy_organik || 0);
            this.performanceByClass.confidence_organik = this.toPercent(payload.confidence_organik || 0);
            this.performanceByClass.precision_anorganik = this.toPercent(payload.precision_anorganik || 0);
            this.performanceByClass.recall_anorganik = this.toPercent(payload.recall_anorganik || 0);
            this.performanceByClass.accuracy_anorganik = this.toPercent(payload.accuracy_anorganik || 0);
            this.performanceByClass.confidence_anorganik = this.toPercent(payload.confidence_anorganik || 0);
        }
    },
    beforeUnmount() {
        socket.off("new_data");
        socket.off("connect");
        socket.off("disconnect");
    }
}
</script>


<style scoped>
@keyframes ping {

    75%,
    100% {
        transform: scale(2);
        opacity: 0;
    }
}

.animate-ping {
    animation: ping 1s cubic-bezier(0, 0, 0.2, 1) infinite;
}
</style>
