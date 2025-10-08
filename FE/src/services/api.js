import axios from 'axios';

const api = axios.create({
  baseURL: '/api',
  timeout: 15000,
});

export async function getSamples(params = {}) {
  const res = await api.get('/samples', { params });
  return res.data || [];
}

export async function getHistory() {
  const res = await api.get('/history');
  return res.data || [];
}

export async function clearHistory() {
  const res = await api.delete('/clear_history');
  return res.data;
}

export default api;
