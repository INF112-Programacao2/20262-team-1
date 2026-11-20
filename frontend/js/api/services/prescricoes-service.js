import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const emitirPrescricao = data => api.post(API_ENDPOINTS.prescricoes, { action: 'emitir_prescricao', data });
export const consultarPrescricoes = pacienteId => api.get(`${API_ENDPOINTS.prescricoes}?paciente_id=${encodeURIComponent(pacienteId)}`);
