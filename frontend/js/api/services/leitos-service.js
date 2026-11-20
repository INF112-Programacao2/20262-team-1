import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const listarLeitos = () => api.get(API_ENDPOINTS.leitos);
export const alocarPaciente = data => api.post(`${API_ENDPOINTS.leitos}/alocacoes`, { action: 'alocar_paciente', data });
export const alterarStatusLeito = data => api.put(`${API_ENDPOINTS.leitos}/${encodeURIComponent(data.id)}`, { action: 'alterar_status_leito', data });
