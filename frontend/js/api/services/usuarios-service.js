import { api } from '../api.js';
import { API_ENDPOINTS } from '../api-endpoints.js';
const action = (name, data) => api.post(API_ENDPOINTS.usuarios, { action: name, data });
export const listarUsuarios = () => api.get(API_ENDPOINTS.usuarios);
export const cadastrarUsuario = data => action('cadastrar_usuario', data);
export const editarUsuario = data => action('editar_usuario', data);
export const excluirUsuario = id => api.delete(`${API_ENDPOINTS.usuarios}/${encodeURIComponent(id)}`);
export const alterarStatusUsuario = data => action('alterar_status_usuario', data);
