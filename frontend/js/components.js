const style = document.createElement('link');
style.rel = 'stylesheet';
style.href = new URL('../css/components.css', import.meta.url).href;
document.head.appendChild(style);

function ensureToastRegion() {
  let region = document.querySelector('.toast-region');
  if (!region) { region = document.createElement('div'); region.className = 'toast-region'; document.body.appendChild(region); }
  return region;
}

export function notify(message, type = 'success') {
  const toast = document.createElement('div');
  toast.className = `toast ${type}`;
  toast.textContent = message;
  ensureToastRegion().appendChild(toast);
  window.setTimeout(() => toast.remove(), 5000);
}

export function openModal({ title, content, confirmLabel = 'Salvar', onConfirm, cancelLabel = 'Cancelar' }) {
  const backdrop = document.createElement('div');
  backdrop.className = 'modal-backdrop';
  backdrop.innerHTML = `<section class="modal" role="dialog" aria-modal="true" aria-labelledby="modal-title"><header class="modal-header"><h2 id="modal-title"></h2><button class="modal-close" type="button" aria-label="Fechar">×</button></header><div class="modal-body"></div><footer class="modal-footer"><button type="button" class="btn btn-secondary modal-cancel">${cancelLabel}</button><button type="button" class="btn modal-confirm">${confirmLabel}</button></footer></section>`;
  backdrop.querySelector('#modal-title').textContent = title;
  backdrop.querySelector('.modal-body').append(typeof content === 'string' ? document.createRange().createContextualFragment(content) : content);
  document.body.appendChild(backdrop);
  const modal = backdrop.querySelector('.modal');
  let escape;
  const close = () => { if (!modal.classList.contains('modal-loading')) { backdrop.remove(); if (escape) document.removeEventListener('keydown', escape); } };
  const confirm = async () => {
    const form = modal.querySelector('form');
    if (form && !form.reportValidity()) return;
    modal.classList.add('modal-loading');
    backdrop.querySelector('.modal-confirm').textContent = 'Aguardando servidor...';
    try { await onConfirm?.(form ? Object.fromEntries(new FormData(form)) : undefined, { modal, backdrop, close }); }
    catch (error) {
      modal.classList.remove('modal-loading');
      backdrop.querySelector('.modal-confirm').textContent = confirmLabel;
      const feedback = document.createElement('div');
      feedback.className = 'modal-feedback visible error';
      feedback.textContent = error.message || 'Não foi possível concluir a operação.';
      modal.querySelector('.modal-body').appendChild(feedback);
    }
  };
  backdrop.querySelector('.modal-confirm').addEventListener('click', confirm);
  backdrop.querySelector('.modal-close').addEventListener('click', close);
  backdrop.querySelector('.modal-cancel').addEventListener('click', close);
  backdrop.addEventListener('click', event => { if (event.target === backdrop) close(); });
  escape = event => { if (event.key === 'Escape') close(); };
  document.addEventListener('keydown', escape);
  modal.querySelector('input,select,textarea,button')?.focus();
  return { close, modal, backdrop };
}

export function confirmAction(options) {
  return new Promise(resolve => openModal({
    ...options,
    confirmLabel: options.confirmLabel || 'Confirmar',
    onConfirm: async (...args) => { await options.onConfirm?.(...args); resolve(true); }
  }));
}
