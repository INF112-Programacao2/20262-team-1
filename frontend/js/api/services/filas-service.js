import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const consultarFila = setor => api.get(`${API_ENDPOINTS.filas}${setor ? `?setor=${encodeURIComponent(setor)}` : ''}`);
export const chamarProximo = data => api.post(`${API_ENDPOINTS.filas}/proximo`, { action: 'chamar_proximo', data });
