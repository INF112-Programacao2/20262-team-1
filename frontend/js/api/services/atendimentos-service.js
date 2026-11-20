import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const listarAtendimentos = () => api.get(API_ENDPOINTS.atendimentos);
export const atualizarStatusAtendimento = data => api.put(`${API_ENDPOINTS.atendimentos}/${encodeURIComponent(data.id)}`, { action: 'atualizar_status_atendimento', data });
