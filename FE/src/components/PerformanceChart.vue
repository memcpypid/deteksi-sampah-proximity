<template>
  <section class="bg-white rounded-2xl shadow-lg p-6 md:p-8 border border-slate-200">
    <div class="flex items-start justify-between mb-6">
      <div class="flex items-center">
        <div class="bg-gradient-to-br from-indigo-500 to-blue-600 p-2.5 rounded-xl mr-3 shadow-sm">
          <svg class="w-5 h-5 text-white" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M11 5h2m-1 0v14m-7-7h14" />
          </svg>
        </div>
        <div>
          <h3 class="text-lg md:text-xl font-bold text-slate-800">Grafik Performa Per Kelas</h3>
          <p class="text-xs md:text-sm text-slate-500">Perbandingan Precision, Recall, Accuracy & Confidence</p>
        </div>
      </div>
      <div class="hidden sm:flex items-center gap-3 text-xs text-slate-500">
        <span class="inline-flex items-center"><span class="w-2.5 h-2.5 rounded-full mr-2" :style="{background: gradientPreview.organ || '#10b981'}"></span>Organik</span>
        <span class="inline-flex items-center"><span class="w-2.5 h-2.5 rounded-full mr-2" :style="{background: gradientPreview.anorgan || '#3b82f6'}"></span>Anorganik</span>
      </div>
    </div>

    <div class="chart-container">
      <Bar ref="chartRef" :data="chartData" :options="chartOptions" />
    </div>
  </section>
</template>

<script>
import { Bar } from 'vue-chartjs'
import {
  Chart as ChartJS,
  Title,
  Tooltip,
  Legend,
  BarElement,
  CategoryScale,
  LinearScale
} from 'chart.js'

ChartJS.register(Title, Tooltip, Legend, BarElement, CategoryScale, LinearScale)

export default {
  name: 'PerformanceChart',
  components: { Bar },
  props: {
    performance: { type: Object, required: true }
  },
  data() {
    return {
      gradients: { organ: null, anorgan: null },
      gradientPreview: { organ: null, anorgan: null }
    }
  },
  computed: {
    chartData() {
      return {
        labels: ['Precision', 'Recall', 'Accuracy', 'Confidence'],
        datasets: [
          {
            label: 'Organik',
            backgroundColor: this.gradients.organ || '#10b981',
            borderColor: '#0ea672',
            borderWidth: 1,
            borderRadius: 8,
            data: [
              this.performance.precision_organik || 0,
              this.performance.recall_organik || 0,
              this.performance.accuracy_organik || 0,
              this.performance.confidence_organik || 0
            ]
          },
          {
            label: 'Anorganik',
            backgroundColor: this.gradients.anorgan || '#3b82f6',
            borderColor: '#2f6bd6',
            borderWidth: 1,
            borderRadius: 8,
            data: [
              this.performance.precision_anorganik || 0,
              this.performance.recall_anorganik || 0,
              this.performance.accuracy_anorganik || 0,
              this.performance.confidence_anorganik || 0
            ]
          }
        ]
      }
    },
    chartOptions() {
      return {
        responsive: true,
        maintainAspectRatio: false,
        layout: { padding: { top: 8, right: 8, bottom: 8, left: 8 } },
        scales: {
          y: {
            beginAtZero: true,
            max: 100,
            grid: { color: '#e2e8f0', drawBorder: false },
            ticks: {
              color: '#475569',
              callback: (val) => `${val}%`
            },
            title: { display: true, text: 'Persentase (%)', color: '#475569' }
          },
          x: {
            grid: { display: false, drawBorder: false },
            ticks: { color: '#475569' },
            title: { display: false }
          }
        },
        plugins: {
          legend: {
            position: 'bottom',
            labels: { color: '#334155', usePointStyle: true, pointStyle: 'round' }
          },
          title: {
            display: false
          },
          tooltip: {
            backgroundColor: 'rgba(15, 23, 42, 0.9)',
            borderColor: '#0ea5e9',
            borderWidth: 0,
            titleColor: '#e2e8f0',
            bodyColor: '#f8fafc',
            padding: 10,
            callbacks: {
              label: (ctx) => `${ctx.dataset.label}: ${ctx.parsed.y}%`
            }
          }
        },
        categoryPercentage: 0.6,
        barPercentage: 0.7,
        maxBarThickness: 36
      }
    }
  },
  mounted() {
    this.$nextTick(() => {
      this.updateGradients()
      window.addEventListener('resize', this.updateGradients)
    })
  },
  beforeUnmount() {
    window.removeEventListener('resize', this.updateGradients)
  },
  methods: {
    updateGradients() {
      const chart = this.$refs.chartRef && this.$refs.chartRef.chart
      if (!chart) return
      const ctx = chart.ctx
      const h = chart.chartArea ? chart.chartArea.bottom - chart.chartArea.top : chart.height

      const gradOrgan = ctx.createLinearGradient(0, 0, 0, h)
      gradOrgan.addColorStop(0, 'rgba(16, 185, 129, 0.9)')
      gradOrgan.addColorStop(1, 'rgba(16, 185, 129, 0.4)')

      const gradAnorgan = ctx.createLinearGradient(0, 0, 0, h)
      gradAnorgan.addColorStop(0, 'rgba(59, 130, 246, 0.9)')
      gradAnorgan.addColorStop(1, 'rgba(59, 130, 246, 0.4)')

      this.gradients.organ = gradOrgan
      this.gradients.anorgan = gradAnorgan
      this.gradientPreview.organ = '#10b981'
      this.gradientPreview.anorgan = '#3b82f6'

      chart.update('none')
    }
  }
}
</script>

<style scoped>
.chart-container { height: 360px; }
</style>
