import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const listarRecursos = () => api.get(API_ENDPOINTS.recursos);
export const atualizarDisponibilidade = data => api.put(`${API_ENDPOINTS.recursos}/${encodeURIComponent(data.id)}`, { action: 'atualizar_disponibilidade', data });
