import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
export const consultarSetores = () => api.get(API_ENDPOINTS.setores);
export const consultarOcupacao = () => api.get(`${API_ENDPOINTS.setores}/ocupacao`);
