(function () {
  const profiles = {
    atendente: { label: 'Atendente', icon: 'AT', dashboard: 'pages/atendente/dashboard.html' },
    medico: { label: 'Médico', icon: 'MD', dashboard: 'pages/medico/dashboard.html' },
    enfermeiro: { label: 'Enfermeiro', icon: 'EN', dashboard: 'pages/enfermeiro/dashboard.html' },
    acompanhante: { label: 'Acompanhante', icon: 'AC', dashboard: 'pages/acompanhante/dashboard.html' },
    administrador: { label: 'Administrador', icon: 'AD', dashboard: 'pages/administrador/dashboard.html' }
  };
  window.VidaCareAuth = {
    profiles,
    select(role) { sessionStorage.setItem('vidacare-role', role); window.location.href = profiles[role].dashboard; },
    current() { return sessionStorage.getItem('vidacare-role'); },
    require() {
      const role = this.current();
      if (!role || !profiles[role]) {
        const fromProfile = window.location.pathname.endsWith('/pages/perfil.html');
        window.location.href = fromProfile ? '../index.html' : '../../index.html';
        return null;
      }
      return role;
    },
    logout() {
      sessionStorage.removeItem('vidacare-role');
      window.location.href = this.isRoot() ? 'index.html' : (window.location.pathname.endsWith('/pages/perfil.html') ? '../index.html' : '../../index.html');
    },
    isRoot() { return window.location.pathname.endsWith('/frontend/index.html') || window.location.pathname.endsWith('/frontend/'); }
  };
})();
