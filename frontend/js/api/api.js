import { API_BASE_URL, API_TIMEOUT_MS } from './api-config.js';
import { ApiError, unavailableError } from './api-errors.js';

async function request(method, path, body) {
  const controller = new AbortController();
  const timer = window.setTimeout(() => controller.abort(), API_TIMEOUT_MS);
  const options = { method, headers: { Accept: 'application/json' }, signal: controller.signal };
  if (body !== undefined) {
    options.headers['Content-Type'] = 'application/json';
    options.body = JSON.stringify(body);
  }
  try {
    const response = await fetch(`${API_BASE_URL}${path}`, options);
    const text = await response.text();
    let payload;
    try { payload = text ? JSON.parse(text) : {}; } catch { throw new ApiError('O servidor retornou uma resposta JSON inválida.', response.status, 'INVALID_JSON'); }
    if (!response.ok || payload.success === false) {
      throw new ApiError(payload.message || `A operação falhou (HTTP ${response.status}).`, response.status, payload.code || 'HTTP_ERROR');
    }
    return payload;
  } catch (error) {
    if (error.name === 'AbortError' || error instanceof TypeError) throw unavailableError();
    throw error;
  } finally {
    window.clearTimeout(timer);
  }
}

export const api = {
  get: path => request('GET', path),
  post: (path, data) => request('POST', path, data),
  put: (path, data) => request('PUT', path, data),
  delete: path => request('DELETE', path)
};
