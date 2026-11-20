import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const listarPacientes = () => api.get(API_ENDPOINTS.pacientes);
export const cadastrarPaciente = data => api.post(API_ENDPOINTS.pacientes, { action: 'cadastrar_paciente', data });
export const editarPaciente = data => api.post(API_ENDPOINTS.pacientes, { action: 'editar_paciente', data });
export const excluirPaciente = id => api.delete(`${API_ENDPOINTS.pacientes}/${encodeURIComponent(id)}`);
