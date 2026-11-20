import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const consultarRelatorio = tipo => api.get(`${API_ENDPOINTS.relatorios}?tipo=${encodeURIComponent(tipo)}`);
export const exportarRelatorio = data => api.post(`${API_ENDPOINTS.relatorios}/exportacoes`, { action: 'exportar_relatorio', data });
