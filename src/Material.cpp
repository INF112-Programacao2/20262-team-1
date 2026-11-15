
#include "../include/Material.h"
Material::Material(int id, std::string nome, const std::string descricao, std::string unidadeMedida, double quantidade) : RecursoHospitalar(id, nome, descricao){
    m_unidadeMedida=unidadeMedida;
    m_quantidade=quantidade;
}

void Material::retirar(double qtd) {
    if (qtd > 0 && qtd <= m_quantidade) {
        m_quantidade -= qtd;
    }
}

void Material::reabastecer(double qtd) {
    if (qtd > 0) {
        m_quantidade += qtd;
    }
}

double Material::getQuantidade() {
    return m_quantidade;
}
