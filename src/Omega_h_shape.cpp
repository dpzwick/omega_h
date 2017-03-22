#include "Omega_h_shape.hpp"

#include "Omega_h_loop.hpp"

namespace Omega_h {

template <typename EdgeLengths>
static Reals measure_edges_tmpl(Mesh* mesh, LOs a2e) {
  EdgeLengths measurer(mesh);
  auto ev2v = mesh->ask_verts_of(EDGE);
  auto na = a2e.size();
  Write<Real> lengths(na);
  auto f = OMEGA_H_LAMBDA(LO a) {
    auto e = a2e[a];
    auto v = gather_verts<2>(ev2v, e);
    lengths[a] = measurer.measure(v);
  };
  parallel_for(na, f);
  return lengths;
}

Reals measure_edges_real(Mesh* mesh, LOs a2e) {
  if (mesh->dim() == 3) {
    return measure_edges_tmpl<RealEdgeLengths<3>>(mesh, a2e);
  } else {
    OMEGA_H_CHECK(mesh->dim() == 2);
    return measure_edges_tmpl<RealEdgeLengths<2>>(mesh, a2e);
  }
}

Reals measure_edges_metric(Mesh* mesh, LOs a2e) {
  auto metrics = mesh->get_array<Real>(VERT, "metric");
  auto metric_dim = get_metrics_dim(mesh->nverts(), metrics);
  if (mesh->dim() == 3 && metric_dim == 3) {
    return measure_edges_tmpl<MetricEdgeLengths<3, 3>>(mesh, a2e);
  }
  if (mesh->dim() == 2 && metric_dim == 2) {
    return measure_edges_tmpl<MetricEdgeLengths<2, 2>>(mesh, a2e);
  }
  if (mesh->dim() == 3 && metric_dim == 1) {
    return measure_edges_tmpl<MetricEdgeLengths<3, 1>>(mesh, a2e);
  }
  if (mesh->dim() == 2 && metric_dim == 1) {
    return measure_edges_tmpl<MetricEdgeLengths<2, 1>>(mesh, a2e);
  }
  OMEGA_H_NORETURN(Reals());
}

Reals measure_edges_real(Mesh* mesh) {
  return measure_edges_real(mesh, LOs(mesh->nedges(), 0, 1));
}

Reals measure_edges_metric(Mesh* mesh) {
  return measure_edges_metric(mesh, LOs(mesh->nedges(), 0, 1));
}

template <Int dim>
static Reals measure_elements_real_tmpl(Mesh* mesh) {
  RealElementSizes measurer(mesh);
  auto ev2v = mesh->ask_elem_verts();
  auto ne = mesh->nelems();
  Write<Real> sizes(ne);
  auto f = OMEGA_H_LAMBDA(LO e) {
    auto v = gather_verts<dim + 1>(ev2v, e);
    sizes[e] = measurer.measure(v);
  };
  parallel_for(ne, f);
  return sizes;
}

Reals measure_elements_real(Mesh* mesh) {
  if (mesh->dim() == 3) return measure_elements_real_tmpl<3>(mesh);
  if (mesh->dim() == 2) return measure_elements_real_tmpl<2>(mesh);
  OMEGA_H_NORETURN(Reals());
}

}  // end namespace Omega_h