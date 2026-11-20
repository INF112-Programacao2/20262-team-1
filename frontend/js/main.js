(function () {
  const roles = [
    ['atendente', 'Atendente'], ['medico', 'Médico'], ['enfermeiro', 'Enfermeiro'],
    ['acompanhante', 'Acompanhante'], ['administrador', 'Administrador']
  ];
  const list = document.querySelector('#role-list');
  if (list) list.innerHTML = roles.map(([key, label]) =>
    `<button class="role-card" data-role="${key}"><span class="role-icon">${VidaCareAuth.profiles[key].icon}</span><span>${label}</span></button>`
  ).join('');
  document.querySelectorAll('[data-role]').forEach(button => button.addEventListener('click', () => VidaCareAuth.select(button.dataset.role)));
})();
