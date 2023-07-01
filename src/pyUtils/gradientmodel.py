"""Calculate the properties of a rough metal surface using the Gradient Model.

References:

   G. Gold and K. Helmreich, “A Physical Surface Roughness Model and Its
   Applications,” IEEE Trans. Microw. Theory Tech., vol. 65, no. 10, pp.
   3720–3732, Oct. 2017, doi: 10.1109/TMTT.2017.2695192.

   K. Lomakin, G. Gold, and K. Helmreich, “Analytical Waveguide Model
   Precisely Predicting Loss and Delay Including Surface Roughness,” IEEE
   Trans. Microw. Theory Tech., vol. 66, no. 6, pp. 2649–2662, Jun. 2018,
   doi: 10.1109/TMTT.2018.2827383.

This package uses the closed-form solution from:

   D. N. Grujic, “Closed-Form Solution of Rough Conductor Surface Impedance,”
   IEEE Trans. Microw. Theory Tech., vol. 66, no. 11, pp. 4677–4683, 2018,
   doi: 10.1109/TMTT.2018.2864586.

"""

import numpy as np
from mpmath import hyp2f1, hyp3f2
from numpy import ndarray, pi, sqrt, exp
from scipy.constants import mu_0, epsilon_0


def mag_field(x, f, rq, sigma0=5.8e7):
    """Calculate the magnetic field tangential to the metal surface.
    Args:
        x: position relative to surface, in units [m]
        f: frequency, in units [Hz]
        rq: rms surface roughness, in units [m]
        sigma0: dc conductivity, optional, default is 5.8e7, in units [S]

    Returns:
        magnetic field
    """
    x_is_array = isinstance(x, ndarray) and len(x) > 1
    f_is_array = isinstance(f, ndarray) and len(f) > 1
    if x_is_array and f_is_array:
        print("x and f can't both be arrays")
        raise ValueError
    # Constants
    xi = 0.5
    chi = sqrt(2) * rq
    # Angular frequency
    w = 2 * pi * f
    # Eqns 15 and 21 from Grujic 2018
    alpha = (1 + 1j) / 2 * rq * sqrt(mu_0 * w * sigma0)
    beta = 0.5 * (sqrt(1 + 4 * alpha ** 2) - 1)
    # Eqn 32 from Grujic 2018
    zeta = 1 / (1 + exp(2 * (x / chi + xi)))
    # Coefficients
    a1 = alpha + beta
    a2 = alpha - beta - 1
    b1 = 1 + 2 * alpha
    # Eqn 31 from Grujic 2018
    if isinstance(x, ndarray) and len(x) > 1:
        mag = np.empty_like(x, dtype=complex)
        for i, _z in np.ndenumerate(zeta):
            mag[i] = _z ** alpha
            mag[i] *= hyp2f1(a1, a2, b1, _z)
    elif isinstance(f, ndarray) and len(f) > 1:
        mag = np.empty_like(f, dtype=complex)
        for i in range(len(f)):
            mag[i] = zeta ** alpha[i] * hyp2f1(a1[i], a2[i], b1[i], zeta)
    else:
        mag = zeta ** alpha * hyp2f1(a1, a2, b1, zeta)
    return mag


def surface_impedance(f, rq, x0=None, sigma0=5.8e7):
    """Calculate the surface impedance of a rough metal.
    Args:
        f: frequency, in units [Hz]
        rq: rms surface roughness, in units [m]
        x0: starting point for integral, optional, default is -5*rq,
            in units [m]
        sigma0: dc conductivity, optional, default is 5.8e7, in units [S]

    Returns:
        surface impedance
    """
    f_is_array = isinstance(f, ndarray) and len(f) > 1
    # Constants
    xi = 0.5
    chi = sqrt(2) * rq
    if x0 is None:
        x0 = -5 * rq
    # Angular frequency
    w = 2 * pi * f
    # Eqns 15 and 21
    alpha = (1 + 1j) / 2 * rq * sqrt(mu_0 * w * sigma0)
    beta = 0.5 * (sqrt(1 + 4 * alpha ** 2) - 1)
    # Eqn 32
    zeta = 1 / (1 + exp(2 * (x0 / chi + xi)))
    # Magnetic field, mag
    mag = mag_field(x0, f, rq, sigma0=sigma0)
    # Anti-derivative, bb
    # Eqn 40 and 41 in Grujic 2018
    a1 = 1 + alpha - beta
    a2 = 2 + alpha + beta
    a3 = alpha
    b1 = 1 + 2 * alpha
    b2 = 1 + alpha
    if f_is_array:
        f0 = np.empty_like(f, dtype=complex)
        f1 = np.empty_like(f, dtype=complex)
        for i in range(len(f)):
            f1[i] = hyp3f2(a1[i], a2[i], a3[i] + 1, b1[i], b2[i] + 1, zeta)
            f0[i] = hyp3f2(a1[i], a2[i], a3[i], b1[i], b2[i], zeta)
        bb = chi / 2 * (zeta ** alpha) * (zeta / (1 + alpha) * f1 - f0 / alpha)
    else:
        f1 = hyp3f2(a1, a2, a3 + 1, b1, b2 + 1, zeta)
        f0 = hyp3f2(a1, a2, a3, b1, b2, zeta)
        bb = chi / 2 * (zeta ** alpha) * (zeta / (1 + alpha) * f1 - f0 / alpha)
    return -1j * mu_0 * w * bb / mag


def rough_properties(f, rq, x0=None, sigma0=5.8e7):
    """Calculate the surface properties of a rough metal.
    Args:
        f: frequency, in units [Hz]
        rq: rms surface roughness, in units [m]
        x0: starting point for integral, optional, default is -5*rq,
            in units [m]
        sigma0: dc conductivity, optional, default is 5.8e7, in units [S]

    Returns:
        surface impedance, effective conductivity, effective permeability
    """
    # Angular frequency
    w = 2 * pi * f
    # Surface roughness
    zs_rough = surface_impedance(f, rq, x0=x0, sigma0=sigma0)
    # Effective conductivity
    cond_eff = mu_0 * w / (2 * zs_rough.real ** 2)
    # Effective permeability
    ur_eff = 2 * sigma0 * zs_rough.imag ** 2 / w / mu_0
    return zs_rough, cond_eff, ur_eff


# Waveguide loss -------------------------------------------------------------

def waveguide_propagation(f, a, b, cond_eff, ur_eff, cond=5.8e7, er=1, ur=1, tand=0):
    # angular frequency
    w = 2 * pi * f
    # skin depths
    skin_c = 1 / sqrt(pi * mu_0 * ur * cond_eff * f)
    skin_m = 1 / sqrt(pi * mu_0 * ur_eff * cond * f)
    # equivalent circuit values
    r1 = 2 / (cond_eff * skin_c * b)
    r2 = 2 * a * (a + 2 * b) / (b * pi**2 * cond_eff * skin_c)
    l1 = 2 / (cond * skin_m * b * a)
    l2 = 2 * a * (a + 2 * b) / (b * pi**2 * cond * skin_m * a)
    c1 = er * epsilon_0
    g1 = w * c1 * tand
    # propagation constant
    t1 = r1 + 1j * w * l1
    t2 = 1 / (r2 + 1j * w * l2) + g1 + 1j * w * c1
    gamma = sqrt(t1 * t2)
    # characteristic impedance
    zte = sqrt(t1 / t2)
    return gamma, zte

