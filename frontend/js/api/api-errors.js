export class ApiError extends Error {
  constructor(message, status = 0, code = 'API_ERROR') {
    super(message);
    this.name = 'ApiError';
    this.status = status;
    this.code = code;
  }
}

export function unavailableError() {
  return new ApiError('Backend indisponível. Verifique se o servidor C++ está em execução e tente novamente.', 0, 'BACKEND_UNAVAILABLE');
}
