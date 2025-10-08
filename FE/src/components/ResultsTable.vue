<template>
  <section class="bg-white rounded-2xl shadow-lg p-6 md:p-8 border border-slate-200">
    <div class="flex items-center justify-between mb-6">
      <div class="flex items-center">
        <div class="bg-gradient-to-br from-emerald-500 to-teal-600 p-2.5 rounded-xl mr-3 shadow-sm">
          <svg class="w-5 h-5 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M3 7h18M3 12h18M3 17h18" />
          </svg>
        </div>
        <div>
          <h3 class="text-lg md:text-xl font-bold text-slate-800">Data Terekam</h3>
          <p class="text-xs md:text-sm text-slate-500">Riwayat data yang sudah direkam</p>
        </div>
      </div>
      <div class="flex flex-col sm:flex-row items-stretch sm:items-center gap-2 sm:gap-3 w-full">
        <div class="flex flex-col sm:flex-row gap-2 sm:items-center sm:mr-3 w-full sm:w-auto">
          <input v-model="fromDate" type="date" class="border border-slate-300 rounded-md text-sm p-2 w-full sm:w-auto" />
          <span class="text-slate-500 text-xs text-center sm:text-left">s/d</span>
          <input v-model="toDate" type="date" class="border border-slate-300 rounded-md text-sm p-2 w-full sm:w-auto" />
          <button @click="applyFilter" :disabled="loading" class="w-full sm:w-auto px-3 py-2 rounded-lg text-sm font-medium text-white bg-emerald-600 hover:bg-emerald-700 disabled:opacity-50">Terapkan</button>
          <button @click="resetFilter" :disabled="loading" class="w-full sm:w-auto px-3 py-2 rounded-lg text-sm font-medium text-slate-700 bg-slate-100 hover:bg-slate-200 disabled:opacity-50">Reset</button>
        </div>
        <button @click="refresh" :disabled="loading" class="w-full sm:w-auto px-3 py-2 rounded-lg text-sm font-medium text-white bg-indigo-600 hover:bg-indigo-700 disabled:opacity-50">Muat Ulang</button>
        <button @click="onClear" :disabled="loading || rows.length===0" class="w-full sm:w-auto px-3 py-2 rounded-lg text-sm font-medium text-white bg-rose-600 hover:bg-rose-700 disabled:opacity-50">Hapus Semua</button>
      </div>
    </div>

    <div v-if="error" class="p-3 mb-4 rounded-md bg-rose-50 text-rose-700 border border-rose-200 text-sm">{{ error }}</div>

    <div class="overflow-x-auto">
      <table class="min-w-full text-sm">
        <thead>
          <tr class="text-left text-slate-600 border-b border-slate-200">
            <th class="py-3 pr-4">ID</th>
            <th class="py-3 pr-4">Waktu</th>
            <th class="py-3 pr-4">Humidity</th>
            <th class="py-3 pr-4">Induktif</th>
            <th class="py-3 pr-4">Kapasitif</th>
            <th class="py-3 pr-4">Prediksi</th>
            <th class="py-3 pr-4">Aktual</th>
            <th class="py-3 pr-4">TP Org</th>
            <th class="py-3 pr-4">TN Org</th>
            <th class="py-3 pr-4">FP Org</th>
            <th class="py-3 pr-4">FN Org</th>
            <th class="py-3 pr-4">Prec Org</th>
            <th class="py-3 pr-4">Recall Org</th>
            <th class="py-3 pr-4">Acc Org</th>
            <th class="py-3 pr-4">TP Anorg</th>
            <th class="py-3 pr-4">TN Anorg</th>
            <th class="py-3 pr-4">FP Anorg</th>
            <th class="py-3 pr-4">FN Anorg</th>
            <th class="py-3 pr-4">Prec Anorg</th>
            <th class="py-3 pr-4">Recall Anorg</th>
            <th class="py-3 pr-4">Acc Anorg</th>
            <th class="py-3 pr-4">Acc Total</th>
            <th class="py-3 pr-4">Updated</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="row in pagedRows" :key="row.id" class="border-b border-slate-100 hover:bg-slate-50">
            <td class="py-2 pr-4">{{ row.id }}</td>
            <td class="py-2 pr-4 whitespace-nowrap">{{ formatTime(row.createdAt) }}</td>
            <td class="py-2 pr-4">{{ formatNumber(row.humidity) }}%</td>
            <td class="py-2 pr-4">
              <span :class="row.induktif ? 'text-emerald-600' : 'text-slate-500'">{{ row.induktif ? 'Terdeteksi' : 'Tidak' }}</span>
            </td>
            <td class="py-2 pr-4">
              <span :class="row.kapasitif ? 'text-emerald-600' : 'text-slate-500'">{{ row.kapasitif ? 'Terdeteksi' : 'Tidak' }}</span>
            </td>
            <td class="py-2 pr-4 font-medium" :class="row.prediksi === 'ORGANIK' ? 'text-emerald-600' : 'text-blue-600'">{{ row.prediksi || '-' }}</td>
            <td class="py-2 pr-4">{{ row.aktual || '-' }}</td>

            <td class="py-2 pr-4">{{ row.tp_organik ?? '-' }}</td>
            <td class="py-2 pr-4">{{ row.tn_organik ?? '-' }}</td>
            <td class="py-2 pr-4">{{ row.fp_organik ?? '-' }}</td>
            <td class="py-2 pr-4">{{ row.fn_organik ?? '-' }}</td>
            <td class="py-2 pr-4">{{ toPercent(row.precision_organik) }}%</td>
            <td class="py-2 pr-4">{{ toPercent(row.recall_organik) }}%</td>
            <td class="py-2 pr-4">{{ toPercent(row.accuracy_organik) }}%</td>

            <td class="py-2 pr-4">{{ row.tp_anorganik ?? '-' }}</td>
            <td class="py-2 pr-4">{{ row.tn_anorganik ?? '-' }}</td>
            <td class="py-2 pr-4">{{ row.fp_anorganik ?? '-' }}</td>
            <td class="py-2 pr-4">{{ row.fn_anorganik ?? '-' }}</td>
            <td class="py-2 pr-4">{{ toPercent(row.precision_anorganik) }}%</td>
            <td class="py-2 pr-4">{{ toPercent(row.recall_anorganik) }}%</td>
            <td class="py-2 pr-4">{{ toPercent(row.accuracy_anorganik) }}%</td>

            <td class="py-2 pr-4">{{ toPercent(row.accuracy_total) }}%</td>
            <td class="py-2 pr-4 whitespace-nowrap">{{ formatTime(row.updatedAt) }}</td>
          </tr>
          <tr v-if="!loading && rows.length===0">
            <td colspan="7" class="py-6 text-center text-slate-500">Belum ada data</td>
          </tr>
        </tbody>
      </table>
    </div>

    <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-3 mt-4">
      <p class="text-xs text-slate-500">Menampilkan {{ pagedRows.length }} dari {{ rows.length }} data</p>
      <div class="flex items-center gap-2">
        <button @click="prev" :disabled="page===1" class="px-3 py-1.5 rounded-md border border-slate-300 text-slate-700 disabled:opacity-50">Prev</button>
        <span class="text-xs text-slate-600">Hal {{ page }} / {{ totalPages }}</span>
        <button @click="next" :disabled="page===totalPages || totalPages===0" class="px-3 py-1.5 rounded-md border border-slate-300 text-slate-700 disabled:opacity-50">Next</button>
        <select v-model.number="pageSize" class="ml-2 border border-slate-300 rounded-md text-xs p-1.5">
          <option :value="5">5</option>
          <option :value="10">10</option>
          <option :value="20">20</option>
          <option :value="50">50</option>
          <option :value="rows.length || 1">Semua</option>
        </select>
      </div>
    </div>
  </section>
</template>

<script>
import { getSamples, clearHistory } from '../services/api'

export default {
  name: 'ResultsTable',
  data() {
    return { rows: [], loading: false, error: '', page: 1, pageSize: 10, fromDate: '', toDate: '' }
  },
  computed: {
    totalPages() { return Math.ceil(this.rows.length / this.pageSize) },
    pagedRows() {
      const start = (this.page - 1) * this.pageSize
      return this.rows.slice(start, start + this.pageSize)
    }
  },
  mounted() { this.refresh() },
  methods: {
    async refresh() {
      this.loading = true; this.error = ''
      try {
        const params = {}
        if (this.fromDate) params.from = this.toStartOfDayISO(this.fromDate)
        if (this.toDate) params.to = this.toEndOfDayISO(this.toDate)
        const data = await getSamples(params)
        this.rows = Array.isArray(data) ? data : []
        if (this.page > this.totalPages) this.page = Math.max(1, this.totalPages)
      } catch (e) {
        this.error = 'Gagal memuat data terekam. Pastikan server backend berjalan.'
      } finally { this.loading = false }
    },
    async onClear() {
      if (!confirm('Hapus semua riwayat?')) return
      this.loading = true; this.error = ''
      try { await clearHistory(); await this.refresh() } catch (e) { this.error = 'Gagal menghapus riwayat' } finally { this.loading = false }
    },
    prev() { if (this.page > 1) this.page-- },
    next() { if (this.page < this.totalPages) this.page++ },
    applyFilter() { this.page = 1; this.refresh() },
    resetFilter() { this.fromDate = ''; this.toDate = ''; this.page = 1; this.refresh() },
    toPercent(v) { if (v == null || isNaN(v)) return 0; const n = v > 1 ? v : v * 100; return Number(n.toFixed(2)) },
    formatNumber(n) { if (n == null || isNaN(n)) return 0; return Number(n).toFixed(1) },
    formatTime(ts) { try { return new Date(ts).toLocaleString() } catch { return '-' } },
    toStartOfDayISO(d) { const dt = new Date(d + 'T00:00:00'); return dt.toISOString() },
    toEndOfDayISO(d) { const dt = new Date(d + 'T23:59:59.999'); return dt.toISOString() }
  }
}
</script>
