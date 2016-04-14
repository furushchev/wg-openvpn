# wg-openvpn

### install prerequisities

```bash
sudo apt-get build-dep openvpn
```

### build deb from source

```bash
dpkg-buildpackage -uc -b
```

### build deb from deb source

```bash
dpkg-source -x openvpn-XXX.dsc
cd openvpn-XXX
dpkg-buildpackage -uc -b
```

### clean build space

```bash
./debian/rules clean
```

### create patch

```bash
# init patch file
quilt new <patch name>.patch

# edit per file
quilt add /path/to/file
emacs /path/to/file # edit
quilt refresh

# write header to identify what is done in patch file
quilt -e header
quilt refresh

# finalize
quilt pop
```

To test patch

```bash
# apply patch in order written in debian/patches/series
quilt push

# revert patch in reverse order written in debian/patches/series
quilt pop
```
