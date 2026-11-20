import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const listarProcedimentos = () => api.get(API_ENDPOINTS.procedimentos);
export const registrarProcedimento = data => api.post(API_ENDPOINTS.procedimentos, { action: 'registrar_procedimento', data });
export const confirmarExecucao = data => api.put(`${API_ENDPOINTS.procedimentos}/${encodeURIComponent(data.id)}`, { action: 'confirmar_execucao', data });
