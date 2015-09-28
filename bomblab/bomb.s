0000000000401240 <main>:
  401240:	53                   	push   %rbx
  401241:	83 ff 01             	cmp    $0x1,%edi
  401244:	75 10                	jne    401256 <main+0x16>
  401246:	48 8b 05 4b 4a 2d 00 	mov    0x2d4a4b(%rip),%rax        # 6d5c98 <_IO_stdin>
  40124d:	48 89 05 24 84 2d 00 	mov    %rax,0x2d8424(%rip)        # 6d9678 <infile>
  401254:	eb 63                	jmp    4012b9 <main+0x79>
  401256:	48 89 f3             	mov    %rsi,%rbx
  401259:	83 ff 02             	cmp    $0x2,%edi
  40125c:	75 3a                	jne    401298 <main+0x58>
  40125e:	48 8b 7e 08          	mov    0x8(%rsi),%rdi
  401262:	be 30 a0 4a 00       	mov    $0x4aa030,%esi
  401267:	e8 14 3a 00 00       	callq  404c80 <_IO_new_fopen>
  40126c:	48 89 05 05 84 2d 00 	mov    %rax,0x2d8405(%rip)        # 6d9678 <infile>
  401273:	48 85 c0             	test   %rax,%rax
  401276:	75 41                	jne    4012b9 <main+0x79>
  401278:	48 8b 4b 08          	mov    0x8(%rbx),%rcx
  40127c:	48 8b 13             	mov    (%rbx),%rdx
  40127f:	be 24 58 4a 00       	mov    $0x4a5824,%esi
  401284:	bf 01 00 00 00       	mov    $0x1,%edi
  401289:	e8 02 b4 02 00       	callq  42c690 <___printf_chk>
  40128e:	bf 08 00 00 00       	mov    $0x8,%edi
  401293:	e8 e8 25 00 00       	callq  403880 <exit>
  401298:	48 8b 16             	mov    (%rsi),%rdx
  40129b:	be 41 58 4a 00       	mov    $0x4a5841,%esi
  4012a0:	bf 01 00 00 00       	mov    $0x1,%edi
  4012a5:	b8 00 00 00 00       	mov    $0x0,%eax
  4012aa:	e8 e1 b3 02 00       	callq  42c690 <___printf_chk>
  4012af:	bf 08 00 00 00       	mov    $0x8,%edi
  4012b4:	e8 c7 25 00 00       	callq  403880 <exit>
  4012b9:	e8 1f 05 00 00       	callq  4017dd <initialize_bomb>
  4012be:	bf a8 58 4a 00       	mov    $0x4a58a8,%edi
  4012c3:	e8 a8 3d 00 00       	callq  405070 <_IO_puts>
  4012c8:	bf e8 58 4a 00       	mov    $0x4a58e8,%edi
  4012cd:	e8 9e 3d 00 00       	callq  405070 <_IO_puts>
  4012d2:	e8 05 08 00 00       	callq  401adc <read_line>
  4012d7:	48 89 c7             	mov    %rax,%rdi
  4012da:	e8 a1 00 00 00       	callq  401380 <phase_1>
  4012df:	e8 1e 09 00 00       	callq  401c02 <phase_defused>
  4012e4:	bf 18 59 4a 00       	mov    $0x4a5918,%edi
  4012e9:	e8 82 3d 00 00       	callq  405070 <_IO_puts>
  4012ee:	e8 e9 07 00 00       	callq  401adc <read_line>
  4012f3:	48 89 c7             	mov    %rax,%rdi
  4012f6:	e8 a1 00 00 00       	callq  40139c <phase_2>
  4012fb:	e8 02 09 00 00       	callq  401c02 <phase_defused>
  401300:	bf 5b 58 4a 00       	mov    $0x4a585b,%edi
  401305:	e8 66 3d 00 00       	callq  405070 <_IO_puts>
  40130a:	e8 cd 07 00 00       	callq  401adc <read_line>
  40130f:	48 89 c7             	mov    %rax,%rdi
  401312:	e8 c9 00 00 00       	callq  4013e0 <phase_3>
  401317:	e8 e6 08 00 00       	callq  401c02 <phase_defused>
  40131c:	bf 79 58 4a 00       	mov    $0x4a5879,%edi
  401321:	e8 4a 3d 00 00       	callq  405070 <_IO_puts>
  401326:	e8 b1 07 00 00       	callq  401adc <read_line>
  40132b:	48 89 c7             	mov    %rax,%rdi
  40132e:	e8 93 01 00 00       	callq  4014c6 <phase_4>
  401333:	e8 ca 08 00 00       	callq  401c02 <phase_defused>
  401338:	bf 48 59 4a 00       	mov    $0x4a5948,%edi
  40133d:	e8 2e 3d 00 00       	callq  405070 <_IO_puts>
  401342:	e8 95 07 00 00       	callq  401adc <read_line>
  401347:	48 89 c7             	mov    %rax,%rdi
  40134a:	e8 d4 01 00 00       	callq  401523 <phase_5>
  40134f:	e8 ae 08 00 00       	callq  401c02 <phase_defused>
  401354:	bf 88 58 4a 00       	mov    $0x4a5888,%edi
  401359:	e8 12 3d 00 00       	callq  405070 <_IO_puts>
  40135e:	e8 79 07 00 00       	callq  401adc <read_line>
  401363:	48 89 c7             	mov    %rax,%rdi
  401366:	e8 20 02 00 00       	callq  40158b <phase_6>
  40136b:	e8 92 08 00 00       	callq  401c02 <phase_defused>
  401370:	b8 00 00 00 00       	mov    $0x0,%eax
  401375:	5b                   	pop    %rbx
  401376:	c3                   	retq   
  401377:	90                   	nop
  401378:	90                   	nop
  401379:	90                   	nop
  40137a:	90                   	nop
  40137b:	90                   	nop
  40137c:	90                   	nop
  40137d:	90                   	nop
  40137e:	90                   	nop
  40137f:	90                   	nop

0000000000401380 <phase_1>:
  401380:	48 83 ec 08          	sub    $0x8,%rsp
  401384:	be 6c 59 4a 00       	mov    $0x4a596c,%esi
  401389:	e8 02 04 00 00       	callq  401790 <strings_not_equal>
  40138e:	85 c0                	test   %eax,%eax
  401390:	74 05                	je     401397 <phase_1+0x17>
  401392:	e8 cd 06 00 00       	callq  401a64 <explode_bomb>
  401397:	48 83 c4 08          	add    $0x8,%rsp
  40139b:	c3                   	retq   

000000000040139c <phase_2>:
  40139c:	53                   	push   %rbx
  40139d:	48 83 ec 20          	sub    $0x20,%rsp
  4013a1:	48 89 e6             	mov    %rsp,%rsi
  4013a4:	e8 f1 06 00 00       	callq  401a9a <read_six_numbers>
  4013a9:	83 3c 24 01          	cmpl   $0x1,(%rsp)
  4013ad:	74 05                	je     4013b4 <phase_2+0x18>
  4013af:	e8 b0 06 00 00       	callq  401a64 <explode_bomb>
  4013b4:	bb 01 00 00 00       	mov    $0x1,%ebx
  4013b9:	eb 1a                	jmp    4013d5 <phase_2+0x39>
  4013bb:	48 63 d3             	movslq %ebx,%rdx
  4013be:	8d 43 ff             	lea    -0x1(%rbx),%eax
  4013c1:	48 98                	cltq   
  4013c3:	8b 04 84             	mov    (%rsp,%rax,4),%eax
  4013c6:	01 c0                	add    %eax,%eax
  4013c8:	39 04 94             	cmp    %eax,(%rsp,%rdx,4)
  4013cb:	74 05                	je     4013d2 <phase_2+0x36>
  4013cd:	e8 92 06 00 00       	callq  401a64 <explode_bomb>
  4013d2:	83 c3 01             	add    $0x1,%ebx
  4013d5:	83 fb 05             	cmp    $0x5,%ebx
  4013d8:	7e e1                	jle    4013bb <phase_2+0x1f>
  4013da:	48 83 c4 20          	add    $0x20,%rsp
  4013de:	5b                   	pop    %rbx
  4013df:	c3                   	retq   

00000000004013e0 <phase_3>:
  4013e0:	48 83 ec 18          	sub    $0x18,%rsp
  4013e4:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  4013e9:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  4013ee:	be 9e 5c 4a 00       	mov    $0x4a5c9e,%esi
  4013f3:	b8 00 00 00 00       	mov    $0x0,%eax
  4013f8:	e8 03 33 00 00       	callq  404700 <__isoc99_sscanf>
  4013fd:	83 f8 01             	cmp    $0x1,%eax
  401400:	7f 05                	jg     401407 <phase_3+0x27>
  401402:	e8 5d 06 00 00       	callq  401a64 <explode_bomb>
  401407:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)
  40140c:	77 64                	ja     401472 <phase_3+0x92>
  40140e:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401412:	ff 24 c5 80 59 4a 00 	jmpq   *0x4a5980(,%rax,8)
  401419:	b8 1b 01 00 00       	mov    $0x11b,%eax
  40141e:	eb 05                	jmp    401425 <phase_3+0x45>
  401420:	b8 00 00 00 00       	mov    $0x0,%eax
  401425:	83 e8 62             	sub    $0x62,%eax
  401428:	eb 05                	jmp    40142f <phase_3+0x4f>
  40142a:	b8 00 00 00 00       	mov    $0x0,%eax
  40142f:	05 4a 03 00 00       	add    $0x34a,%eax
  401434:	eb 05                	jmp    40143b <phase_3+0x5b>
  401436:	b8 00 00 00 00       	mov    $0x0,%eax
  40143b:	2d 87 00 00 00       	sub    $0x87,%eax
  401440:	eb 05                	jmp    401447 <phase_3+0x67>
  401442:	b8 00 00 00 00       	mov    $0x0,%eax
  401447:	05 87 00 00 00       	add    $0x87,%eax
  40144c:	eb 05                	jmp    401453 <phase_3+0x73>
  40144e:	b8 00 00 00 00       	mov    $0x0,%eax
  401453:	2d 87 00 00 00       	sub    $0x87,%eax
  401458:	eb 05                	jmp    40145f <phase_3+0x7f>
  40145a:	b8 00 00 00 00       	mov    $0x0,%eax
  40145f:	05 87 00 00 00       	add    $0x87,%eax
  401464:	eb 05                	jmp    40146b <phase_3+0x8b>
  401466:	b8 00 00 00 00       	mov    $0x0,%eax
  40146b:	2d 87 00 00 00       	sub    $0x87,%eax
  401470:	eb 0a                	jmp    40147c <phase_3+0x9c>
  401472:	e8 ed 05 00 00       	callq  401a64 <explode_bomb>
  401477:	b8 00 00 00 00       	mov    $0x0,%eax
  40147c:	83 7c 24 08 05       	cmpl   $0x5,0x8(%rsp)
  401481:	7f 06                	jg     401489 <phase_3+0xa9>
  401483:	3b 44 24 0c          	cmp    0xc(%rsp),%eax
  401487:	74 05                	je     40148e <phase_3+0xae>
  401489:	e8 d6 05 00 00       	callq  401a64 <explode_bomb>
  40148e:	48 83 c4 18          	add    $0x18,%rsp
  401492:	c3                   	retq   

0000000000401493 <func4>:
  401493:	53                   	push   %rbx
  401494:	89 d0                	mov    %edx,%eax
  401496:	29 f0                	sub    %esi,%eax
  401498:	89 c3                	mov    %eax,%ebx
  40149a:	c1 eb 1f             	shr    $0x1f,%ebx
  40149d:	01 d8                	add    %ebx,%eax
  40149f:	d1 f8                	sar    %eax
  4014a1:	8d 1c 30             	lea    (%rax,%rsi,1),%ebx
  4014a4:	39 fb                	cmp    %edi,%ebx
  4014a6:	7e 0c                	jle    4014b4 <func4+0x21>
  4014a8:	8d 53 ff             	lea    -0x1(%rbx),%edx
  4014ab:	e8 e3 ff ff ff       	callq  401493 <func4>
  4014b0:	01 d8                	add    %ebx,%eax
  4014b2:	eb 10                	jmp    4014c4 <func4+0x31>
  4014b4:	89 d8                	mov    %ebx,%eax
  4014b6:	39 fb                	cmp    %edi,%ebx
  4014b8:	7d 0a                	jge    4014c4 <func4+0x31>
  4014ba:	8d 73 01             	lea    0x1(%rbx),%esi
  4014bd:	e8 d1 ff ff ff       	callq  401493 <func4>
  4014c2:	01 d8                	add    %ebx,%eax
  4014c4:	5b                   	pop    %rbx
  4014c5:	c3                   	retq   

00000000004014c6 <phase_4>:
  4014c6:	48 83 ec 18          	sub    $0x18,%rsp
  4014ca:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  4014cf:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  4014d4:	be 9e 5c 4a 00       	mov    $0x4a5c9e,%esi
  4014d9:	b8 00 00 00 00       	mov    $0x0,%eax
  4014de:	e8 1d 32 00 00       	callq  404700 <__isoc99_sscanf>
  4014e3:	83 f8 02             	cmp    $0x2,%eax
  4014e6:	75 0d                	jne    4014f5 <phase_4+0x2f>
  4014e8:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4014ec:	85 c0                	test   %eax,%eax
  4014ee:	78 05                	js     4014f5 <phase_4+0x2f>
  4014f0:	83 f8 0e             	cmp    $0xe,%eax
  4014f3:	7e 05                	jle    4014fa <phase_4+0x34>
  4014f5:	e8 6a 05 00 00       	callq  401a64 <explode_bomb>
  4014fa:	ba 0e 00 00 00       	mov    $0xe,%edx
  4014ff:	be 00 00 00 00       	mov    $0x0,%esi
  401504:	8b 7c 24 08          	mov    0x8(%rsp),%edi
  401508:	e8 86 ff ff ff       	callq  401493 <func4>
  40150d:	83 f8 07             	cmp    $0x7,%eax
  401510:	75 07                	jne    401519 <phase_4+0x53>
  401512:	83 7c 24 0c 07       	cmpl   $0x7,0xc(%rsp)
  401517:	74 05                	je     40151e <phase_4+0x58>
  401519:	e8 46 05 00 00       	callq  401a64 <explode_bomb>
  40151e:	48 83 c4 18          	add    $0x18,%rsp
  401522:	c3                   	retq   

0000000000401523 <phase_5>:
  401523:	48 83 ec 18          	sub    $0x18,%rsp
  401527:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  40152c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  401531:	be 9e 5c 4a 00       	mov    $0x4a5c9e,%esi
  401536:	b8 00 00 00 00       	mov    $0x0,%eax
  40153b:	e8 c0 31 00 00       	callq  404700 <__isoc99_sscanf>
  401540:	83 f8 01             	cmp    $0x1,%eax
  401543:	7f 05                	jg     40154a <phase_5+0x27>
  401545:	e8 1a 05 00 00       	callq  401a64 <explode_bomb>
  40154a:	83 64 24 08 0f       	andl   $0xf,0x8(%rsp)
  40154f:	b9 00 00 00 00       	mov    $0x0,%ecx
  401554:	ba 00 00 00 00       	mov    $0x0,%edx
  401559:	eb 12                	jmp    40156d <phase_5+0x4a>
  40155b:	83 c2 01             	add    $0x1,%edx
  40155e:	48 98                	cltq   
  401560:	8b 04 85 c0 59 4a 00 	mov    0x4a59c0(,%rax,4),%eax
  401567:	89 44 24 08          	mov    %eax,0x8(%rsp)
  40156b:	01 c1                	add    %eax,%ecx
  40156d:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401571:	83 f8 0f             	cmp    $0xf,%eax
  401574:	75 e5                	jne    40155b <phase_5+0x38>
  401576:	83 fa 0f             	cmp    $0xf,%edx
  401579:	75 06                	jne    401581 <phase_5+0x5e>
  40157b:	3b 4c 24 0c          	cmp    0xc(%rsp),%ecx
  40157f:	74 05                	je     401586 <phase_5+0x63>
  401581:	e8 de 04 00 00       	callq  401a64 <explode_bomb>
  401586:	48 83 c4 18          	add    $0x18,%rsp
  40158a:	c3                   	retq   

000000000040158b <phase_6>:
  40158b:	41 54                	push   %r12
  40158d:	55                   	push   %rbp
  40158e:	53                   	push   %rbx
  40158f:	48 83 ec 50          	sub    $0x50,%rsp
  401593:	48 89 e6             	mov    %rsp,%rsi
  401596:	e8 ff 04 00 00       	callq  401a9a <read_six_numbers>
  40159b:	bd 00 00 00 00       	mov    $0x0,%ebp
  4015a0:	eb 3a                	jmp    4015dc <phase_6+0x51>
  4015a2:	48 63 c5             	movslq %ebp,%rax
  4015a5:	8b 04 84             	mov    (%rsp,%rax,4),%eax
  4015a8:	83 e8 01             	sub    $0x1,%eax
  4015ab:	83 f8 05             	cmp    $0x5,%eax
  4015ae:	76 05                	jbe    4015b5 <phase_6+0x2a>
  4015b0:	e8 af 04 00 00       	callq  401a64 <explode_bomb>
  4015b5:	44 8d 65 01          	lea    0x1(%rbp),%r12d
  4015b9:	44 89 e3             	mov    %r12d,%ebx
  4015bc:	48 63 ed             	movslq %ebp,%rbp
  4015bf:	eb 13                	jmp    4015d4 <phase_6+0x49>
  4015c1:	48 63 c3             	movslq %ebx,%rax
  4015c4:	8b 04 84             	mov    (%rsp,%rax,4),%eax
  4015c7:	39 04 ac             	cmp    %eax,(%rsp,%rbp,4)
  4015ca:	75 05                	jne    4015d1 <phase_6+0x46>
  4015cc:	e8 93 04 00 00       	callq  401a64 <explode_bomb>
  4015d1:	83 c3 01             	add    $0x1,%ebx
  4015d4:	83 fb 05             	cmp    $0x5,%ebx
  4015d7:	7e e8                	jle    4015c1 <phase_6+0x36>
  4015d9:	44 89 e5             	mov    %r12d,%ebp
  4015dc:	83 fd 05             	cmp    $0x5,%ebp
  4015df:	7e c1                	jle    4015a2 <phase_6+0x17>
  4015e1:	be 00 00 00 00       	mov    $0x0,%esi
  4015e6:	eb 26                	jmp    40160e <phase_6+0x83>
  4015e8:	48 8b 52 08          	mov    0x8(%rdx),%rdx
  4015ec:	83 c0 01             	add    $0x1,%eax
  4015ef:	eb 0d                	jmp    4015fe <phase_6+0x73>
  4015f1:	b8 01 00 00 00       	mov    $0x1,%eax
  4015f6:	ba a0 42 6d 00       	mov    $0x6d42a0,%edx
  4015fb:	48 63 ce             	movslq %esi,%rcx
  4015fe:	39 04 8c             	cmp    %eax,(%rsp,%rcx,4)
  401601:	7f e5                	jg     4015e8 <phase_6+0x5d>
  401603:	48 63 c6             	movslq %esi,%rax
  401606:	48 89 54 c4 20       	mov    %rdx,0x20(%rsp,%rax,8)
  40160b:	83 c6 01             	add    $0x1,%esi
  40160e:	83 fe 05             	cmp    $0x5,%esi
  401611:	7e de                	jle    4015f1 <phase_6+0x66>
  401613:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx
  401618:	48 89 d9             	mov    %rbx,%rcx
  40161b:	b8 01 00 00 00       	mov    $0x1,%eax
  401620:	eb 12                	jmp    401634 <phase_6+0xa9>
  401622:	48 63 d0             	movslq %eax,%rdx
  401625:	48 8b 54 d4 20       	mov    0x20(%rsp,%rdx,8),%rdx
  40162a:	48 89 51 08          	mov    %rdx,0x8(%rcx)
  40162e:	83 c0 01             	add    $0x1,%eax
  401631:	48 89 d1             	mov    %rdx,%rcx
  401634:	83 f8 05             	cmp    $0x5,%eax
  401637:	7e e9                	jle    401622 <phase_6+0x97>
  401639:	48 c7 41 08 00 00 00 	movq   $0x0,0x8(%rcx)
  401640:	00 
  401641:	bd 00 00 00 00       	mov    $0x0,%ebp
  401646:	eb 16                	jmp    40165e <phase_6+0xd3>
  401648:	48 8b 43 08          	mov    0x8(%rbx),%rax
  40164c:	8b 00                	mov    (%rax),%eax
  40164e:	39 03                	cmp    %eax,(%rbx)
  401650:	7e 05                	jle    401657 <phase_6+0xcc>
  401652:	e8 0d 04 00 00       	callq  401a64 <explode_bomb>
  401657:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
  40165b:	83 c5 01             	add    $0x1,%ebp
  40165e:	83 fd 04             	cmp    $0x4,%ebp
  401661:	7e e5                	jle    401648 <phase_6+0xbd>
  401663:	48 83 c4 50          	add    $0x50,%rsp
  401667:	5b                   	pop    %rbx
  401668:	5d                   	pop    %rbp
  401669:	41 5c                	pop    %r12
  40166b:	c3                   	retq   

000000000040166c <fun7>:
  40166c:	48 83 ec 08          	sub    $0x8,%rsp
  401670:	48 85 ff             	test   %rdi,%rdi
  401673:	74 2b                	je     4016a0 <fun7+0x34>
  401675:	8b 17                	mov    (%rdi),%edx
  401677:	39 f2                	cmp    %esi,%edx
  401679:	7e 0d                	jle    401688 <fun7+0x1c>
  40167b:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
  40167f:	e8 e8 ff ff ff       	callq  40166c <fun7>
  401684:	01 c0                	add    %eax,%eax
  401686:	eb 1d                	jmp    4016a5 <fun7+0x39>
  401688:	b8 00 00 00 00       	mov    $0x0,%eax
  40168d:	39 f2                	cmp    %esi,%edx
  40168f:	74 14                	je     4016a5 <fun7+0x39>
  401691:	48 8b 7f 10          	mov    0x10(%rdi),%rdi
  401695:	e8 d2 ff ff ff       	callq  40166c <fun7>
  40169a:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
  40169e:	eb 05                	jmp    4016a5 <fun7+0x39>
  4016a0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  4016a5:	48 83 c4 08          	add    $0x8,%rsp
  4016a9:	c3                   	retq   

00000000004016aa <secret_phase>:
  4016aa:	53                   	push   %rbx
  4016ab:	e8 2c 04 00 00       	callq  401adc <read_line>
  4016b0:	ba 0a 00 00 00       	mov    $0xa,%edx
  4016b5:	be 00 00 00 00       	mov    $0x0,%esi
  4016ba:	48 89 c7             	mov    %rax,%rdi
  4016bd:	e8 1e 26 00 00       	callq  403ce0 <strtol>
  4016c2:	48 89 c3             	mov    %rax,%rbx
  4016c5:	8d 40 ff             	lea    -0x1(%rax),%eax
  4016c8:	3d e8 03 00 00       	cmp    $0x3e8,%eax
  4016cd:	76 05                	jbe    4016d4 <secret_phase+0x2a>
  4016cf:	e8 90 03 00 00       	callq  401a64 <explode_bomb>
  4016d4:	89 de                	mov    %ebx,%esi
  4016d6:	bf c0 40 6d 00       	mov    $0x6d40c0,%edi
  4016db:	e8 8c ff ff ff       	callq  40166c <fun7>
  4016e0:	83 f8 02             	cmp    $0x2,%eax
  4016e3:	74 05                	je     4016ea <secret_phase+0x40>
  4016e5:	e8 7a 03 00 00       	callq  401a64 <explode_bomb>
  4016ea:	bf 00 5a 4a 00       	mov    $0x4a5a00,%edi
  4016ef:	e8 7c 39 00 00       	callq  405070 <_IO_puts>
  4016f4:	e8 09 05 00 00       	callq  401c02 <phase_defused>
  4016f9:	5b                   	pop    %rbx
  4016fa:	c3                   	retq   
  4016fb:	90                   	nop
  4016fc:	90                   	nop
  4016fd:	90                   	nop
  4016fe:	90                   	nop
  4016ff:	90                   	nop

0000000000401700 <sig_handler>:
  401700:	48 83 ec 08          	sub    $0x8,%rsp
  401704:	bf 28 5a 4a 00       	mov    $0x4a5a28,%edi
  401709:	e8 62 39 00 00       	callq  405070 <_IO_puts>
  40170e:	bf 03 00 00 00       	mov    $0x3,%edi
  401713:	e8 f8 8a 02 00       	callq  42a210 <__sleep>
  401718:	be 17 5c 4a 00       	mov    $0x4a5c17,%esi
  40171d:	bf 01 00 00 00       	mov    $0x1,%edi
  401722:	b8 00 00 00 00       	mov    $0x0,%eax
  401727:	e8 64 af 02 00       	callq  42c690 <___printf_chk>
  40172c:	48 8b 3d 5d 45 2d 00 	mov    0x2d455d(%rip),%rdi        # 6d5c90 <_IO_stdout>
  401733:	e8 f8 30 00 00       	callq  404830 <_IO_fflush>
  401738:	bf 01 00 00 00       	mov    $0x1,%edi
  40173d:	e8 ce 8a 02 00       	callq  42a210 <__sleep>
  401742:	bf 1f 5c 4a 00       	mov    $0x4a5c1f,%edi
  401747:	e8 24 39 00 00       	callq  405070 <_IO_puts>
  40174c:	bf 10 00 00 00       	mov    $0x10,%edi
  401751:	e8 2a 21 00 00       	callq  403880 <exit>

0000000000401756 <invalid_phase>:
  401756:	48 83 ec 08          	sub    $0x8,%rsp
  40175a:	48 89 fa             	mov    %rdi,%rdx
  40175d:	be 27 5c 4a 00       	mov    $0x4a5c27,%esi
  401762:	bf 01 00 00 00       	mov    $0x1,%edi
  401767:	b8 00 00 00 00       	mov    $0x0,%eax
  40176c:	e8 1f af 02 00       	callq  42c690 <___printf_chk>
  401771:	bf 08 00 00 00       	mov    $0x8,%edi
  401776:	e8 05 21 00 00       	callq  403880 <exit>

000000000040177b <string_length>:
  40177b:	b8 00 00 00 00       	mov    $0x0,%eax
  401780:	eb 07                	jmp    401789 <string_length+0xe>
  401782:	48 83 c7 01          	add    $0x1,%rdi
  401786:	83 c0 01             	add    $0x1,%eax
  401789:	80 3f 00             	cmpb   $0x0,(%rdi)
  40178c:	75 f4                	jne    401782 <string_length+0x7>
  40178e:	f3 c3                	repz retq 

0000000000401790 <strings_not_equal>:
  401790:	41 54                	push   %r12
  401792:	55                   	push   %rbp
  401793:	53                   	push   %rbx
  401794:	48 89 fb             	mov    %rdi,%rbx
  401797:	48 89 f5             	mov    %rsi,%rbp
  40179a:	e8 dc ff ff ff       	callq  40177b <string_length>
  40179f:	41 89 c4             	mov    %eax,%r12d
  4017a2:	48 89 ef             	mov    %rbp,%rdi
  4017a5:	e8 d1 ff ff ff       	callq  40177b <string_length>
  4017aa:	ba 01 00 00 00       	mov    $0x1,%edx
  4017af:	41 39 c4             	cmp    %eax,%r12d
  4017b2:	75 22                	jne    4017d6 <strings_not_equal+0x46>
  4017b4:	eb 0d                	jmp    4017c3 <strings_not_equal+0x33>
  4017b6:	3a 45 00             	cmp    0x0(%rbp),%al
  4017b9:	75 16                	jne    4017d1 <strings_not_equal+0x41>
  4017bb:	48 83 c3 01          	add    $0x1,%rbx
  4017bf:	48 83 c5 01          	add    $0x1,%rbp
  4017c3:	0f b6 03             	movzbl (%rbx),%eax
  4017c6:	84 c0                	test   %al,%al
  4017c8:	75 ec                	jne    4017b6 <strings_not_equal+0x26>
  4017ca:	ba 00 00 00 00       	mov    $0x0,%edx
  4017cf:	eb 05                	jmp    4017d6 <strings_not_equal+0x46>
  4017d1:	ba 01 00 00 00       	mov    $0x1,%edx
  4017d6:	89 d0                	mov    %edx,%eax
  4017d8:	5b                   	pop    %rbx
  4017d9:	5d                   	pop    %rbp
  4017da:	41 5c                	pop    %r12
  4017dc:	c3                   	retq   

00000000004017dd <initialize_bomb>:
  4017dd:	53                   	push   %rbx
  4017de:	48 81 ec 50 20 00 00 	sub    $0x2050,%rsp
  4017e5:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4017ec:	00 00 
  4017ee:	48 89 84 24 48 20 00 	mov    %rax,0x2048(%rsp)
  4017f5:	00 
  4017f6:	31 c0                	xor    %eax,%eax
  4017f8:	be 00 17 40 00       	mov    $0x401700,%esi
  4017fd:	bf 02 00 00 00       	mov    $0x2,%edi
  401802:	e8 a9 1b 00 00       	callq  4033b0 <__bsd_signal>
  401807:	be 40 00 00 00       	mov    $0x40,%esi
  40180c:	48 89 e7             	mov    %rsp,%rdi
  40180f:	e8 cc 91 02 00       	callq  42a9e0 <__gethostname>
  401814:	85 c0                	test   %eax,%eax
  401816:	74 25                	je     40183d <initialize_bomb+0x60>
  401818:	bf 60 5a 4a 00       	mov    $0x4a5a60,%edi
  40181d:	e8 4e 38 00 00       	callq  405070 <_IO_puts>
  401822:	bf 08 00 00 00       	mov    $0x8,%edi
  401827:	e8 54 20 00 00       	callq  403880 <exit>
  40182c:	48 89 e6             	mov    %rsp,%rsi
  40182f:	e8 cc eb ff ff       	callq  400400 <__rela_iplt_end+0x90>
  401834:	85 c0                	test   %eax,%eax
  401836:	74 21                	je     401859 <initialize_bomb+0x7c>
  401838:	83 c3 01             	add    $0x1,%ebx
  40183b:	eb 05                	jmp    401842 <initialize_bomb+0x65>
  40183d:	bb 00 00 00 00       	mov    $0x0,%ebx
  401842:	48 63 c3             	movslq %ebx,%rax
  401845:	48 8b 3c c5 20 53 6d 	mov    0x6d5320(,%rax,8),%rdi
  40184c:	00 
  40184d:	48 85 ff             	test   %rdi,%rdi
  401850:	75 da                	jne    40182c <initialize_bomb+0x4f>
  401852:	b8 00 00 00 00       	mov    $0x0,%eax
  401857:	eb 05                	jmp    40185e <initialize_bomb+0x81>
  401859:	b8 01 00 00 00       	mov    $0x1,%eax
  40185e:	85 c0                	test   %eax,%eax
  401860:	75 14                	jne    401876 <initialize_bomb+0x99>
  401862:	bf 98 5a 4a 00       	mov    $0x4a5a98,%edi
  401867:	e8 04 38 00 00       	callq  405070 <_IO_puts>
  40186c:	bf 08 00 00 00       	mov    $0x8,%edi
  401871:	e8 0a 20 00 00       	callq  403880 <exit>
  401876:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
  40187b:	e8 60 0e 00 00       	callq  4026e0 <init_driver>
  401880:	85 c0                	test   %eax,%eax
  401882:	79 23                	jns    4018a7 <initialize_bomb+0xca>
  401884:	48 8d 54 24 40       	lea    0x40(%rsp),%rdx
  401889:	be 38 5c 4a 00       	mov    $0x4a5c38,%esi
  40188e:	bf 01 00 00 00       	mov    $0x1,%edi
  401893:	b8 00 00 00 00       	mov    $0x0,%eax
  401898:	e8 f3 ad 02 00       	callq  42c690 <___printf_chk>
  40189d:	bf 08 00 00 00       	mov    $0x8,%edi
  4018a2:	e8 d9 1f 00 00       	callq  403880 <exit>
  4018a7:	48 8b 84 24 48 20 00 	mov    0x2048(%rsp),%rax
  4018ae:	00 
  4018af:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4018b6:	00 00 
  4018b8:	74 05                	je     4018bf <initialize_bomb+0xe2>
  4018ba:	e8 21 b3 02 00       	callq  42cbe0 <__stack_chk_fail>
  4018bf:	48 81 c4 50 20 00 00 	add    $0x2050,%rsp
  4018c6:	5b                   	pop    %rbx
  4018c7:	c3                   	retq   

00000000004018c8 <initialize_bomb_solve>:
  4018c8:	f3 c3                	repz retq 

00000000004018ca <blank_line>:
  4018ca:	55                   	push   %rbp
  4018cb:	53                   	push   %rbx
  4018cc:	48 83 ec 08          	sub    $0x8,%rsp
  4018d0:	48 89 fb             	mov    %rdi,%rbx
  4018d3:	eb 17                	jmp    4018ec <blank_line+0x22>
  4018d5:	e8 06 1a 00 00       	callq  4032e0 <__ctype_b_loc>
  4018da:	48 8b 00             	mov    (%rax),%rax
  4018dd:	48 83 c3 01          	add    $0x1,%rbx
  4018e1:	48 0f be ed          	movsbq %bpl,%rbp
  4018e5:	f6 44 68 01 20       	testb  $0x20,0x1(%rax,%rbp,2)
  4018ea:	74 0f                	je     4018fb <blank_line+0x31>
  4018ec:	0f b6 2b             	movzbl (%rbx),%ebp
  4018ef:	40 84 ed             	test   %bpl,%bpl
  4018f2:	75 e1                	jne    4018d5 <blank_line+0xb>
  4018f4:	b8 01 00 00 00       	mov    $0x1,%eax
  4018f9:	eb 05                	jmp    401900 <blank_line+0x36>
  4018fb:	b8 00 00 00 00       	mov    $0x0,%eax
  401900:	48 83 c4 08          	add    $0x8,%rsp
  401904:	5b                   	pop    %rbx
  401905:	5d                   	pop    %rbp
  401906:	c3                   	retq   

0000000000401907 <skip>:
  401907:	53                   	push   %rbx
  401908:	48 63 05 61 53 2d 00 	movslq 0x2d5361(%rip),%rax        # 6d6c70 <num_input_strings>
  40190f:	48 8d 3c 80          	lea    (%rax,%rax,4),%rdi
  401913:	48 c1 e7 04          	shl    $0x4,%rdi
  401917:	48 81 c7 80 96 6d 00 	add    $0x6d9680,%rdi
  40191e:	48 8b 15 53 7d 2d 00 	mov    0x2d7d53(%rip),%rdx        # 6d9678 <infile>
  401925:	be 50 00 00 00       	mov    $0x50,%esi
  40192a:	e8 51 30 00 00       	callq  404980 <_IO_fgets>
  40192f:	48 89 c3             	mov    %rax,%rbx
  401932:	48 85 c0             	test   %rax,%rax
  401935:	74 0c                	je     401943 <skip+0x3c>
  401937:	48 89 c7             	mov    %rax,%rdi
  40193a:	e8 8b ff ff ff       	callq  4018ca <blank_line>
  40193f:	85 c0                	test   %eax,%eax
  401941:	75 c5                	jne    401908 <skip+0x1>
  401943:	48 89 d8             	mov    %rbx,%rax
  401946:	5b                   	pop    %rbx
  401947:	c3                   	retq   

0000000000401948 <send_msg>:
  401948:	48 81 ec 38 40 00 00 	sub    $0x4038,%rsp
  40194f:	41 89 f9             	mov    %edi,%r9d
  401952:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401959:	00 00 
  40195b:	48 89 84 24 28 40 00 	mov    %rax,0x4028(%rsp)
  401962:	00 
  401963:	31 c0                	xor    %eax,%eax
  401965:	8b 35 05 53 2d 00    	mov    0x2d5305(%rip),%esi        # 6d6c70 <num_input_strings>
  40196b:	8d 46 ff             	lea    -0x1(%rsi),%eax
  40196e:	48 98                	cltq   
  401970:	4c 8d 04 80          	lea    (%rax,%rax,4),%r8
  401974:	49 c1 e0 04          	shl    $0x4,%r8
  401978:	49 81 c0 80 96 6d 00 	add    $0x6d9680,%r8
  40197f:	4c 89 c7             	mov    %r8,%rdi
  401982:	b8 00 00 00 00       	mov    $0x0,%eax
  401987:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
  40198e:	f2 ae                	repnz scas %es:(%rdi),%al
  401990:	48 f7 d1             	not    %rcx
  401993:	48 83 c1 63          	add    $0x63,%rcx
  401997:	48 81 f9 00 20 00 00 	cmp    $0x2000,%rcx
  40199e:	76 19                	jbe    4019b9 <send_msg+0x71>
  4019a0:	be d0 5a 4a 00       	mov    $0x4a5ad0,%esi
  4019a5:	bf 01 00 00 00       	mov    $0x1,%edi
  4019aa:	e8 e1 ac 02 00       	callq  42c690 <___printf_chk>
  4019af:	bf 08 00 00 00       	mov    $0x8,%edi
  4019b4:	e8 c7 1e 00 00       	callq  403880 <exit>
  4019b9:	45 85 c9             	test   %r9d,%r9d
  4019bc:	b8 5a 5c 4a 00       	mov    $0x4a5c5a,%eax
  4019c1:	ba 52 5c 4a 00       	mov    $0x4a5c52,%edx
  4019c6:	48 0f 45 c2          	cmovne %rdx,%rax
  4019ca:	4c 89 44 24 10       	mov    %r8,0x10(%rsp)
  4019cf:	89 74 24 08          	mov    %esi,0x8(%rsp)
  4019d3:	48 89 04 24          	mov    %rax,(%rsp)
  4019d7:	41 b9 00 4b 6d 00    	mov    $0x6d4b00,%r9d
  4019dd:	44 8b 05 1c 39 2d 00 	mov    0x2d391c(%rip),%r8d        # 6d5300 <bomb_id>
  4019e4:	b9 63 5c 4a 00       	mov    $0x4a5c63,%ecx
  4019e9:	ba 00 20 00 00       	mov    $0x2000,%edx
  4019ee:	be 01 00 00 00       	mov    $0x1,%esi
  4019f3:	48 8d 7c 24 20       	lea    0x20(%rsp),%rdi
  4019f8:	b8 00 00 00 00       	mov    $0x0,%eax
  4019fd:	e8 1e ab 02 00       	callq  42c520 <___sprintf_chk>
  401a02:	4c 8d 8c 24 20 20 00 	lea    0x2020(%rsp),%r9
  401a09:	00 
  401a0a:	41 b8 00 00 00 00    	mov    $0x0,%r8d
  401a10:	48 8d 4c 24 20       	lea    0x20(%rsp),%rcx
  401a15:	ba 00 43 6d 00       	mov    $0x6d4300,%edx
  401a1a:	be 00 47 6d 00       	mov    $0x6d4700,%esi
  401a1f:	bf 00 4f 6d 00       	mov    $0x6d4f00,%edi
  401a24:	e8 aa 0e 00 00       	callq  4028d3 <driver_post>
  401a29:	85 c0                	test   %eax,%eax
  401a2b:	79 17                	jns    401a44 <send_msg+0xfc>
  401a2d:	48 8d bc 24 20 20 00 	lea    0x2020(%rsp),%rdi
  401a34:	00 
  401a35:	e8 36 36 00 00       	callq  405070 <_IO_puts>
  401a3a:	bf 00 00 00 00       	mov    $0x0,%edi
  401a3f:	e8 3c 1e 00 00       	callq  403880 <exit>
  401a44:	48 8b 84 24 28 40 00 	mov    0x4028(%rsp),%rax
  401a4b:	00 
  401a4c:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  401a53:	00 00 
  401a55:	74 05                	je     401a5c <send_msg+0x114>
  401a57:	e8 84 b1 02 00       	callq  42cbe0 <__stack_chk_fail>
  401a5c:	48 81 c4 38 40 00 00 	add    $0x4038,%rsp
  401a63:	c3                   	retq   

0000000000401a64 <explode_bomb>:
  401a64:	48 83 ec 08          	sub    $0x8,%rsp
  401a68:	bf 72 5c 4a 00       	mov    $0x4a5c72,%edi
  401a6d:	e8 fe 35 00 00       	callq  405070 <_IO_puts>
  401a72:	bf 7b 5c 4a 00       	mov    $0x4a5c7b,%edi
  401a77:	e8 f4 35 00 00       	callq  405070 <_IO_puts>
  401a7c:	bf 00 00 00 00       	mov    $0x0,%edi
  401a81:	e8 c2 fe ff ff       	callq  401948 <send_msg>
  401a86:	bf f8 5a 4a 00       	mov    $0x4a5af8,%edi
  401a8b:	e8 e0 35 00 00       	callq  405070 <_IO_puts>
  401a90:	bf 08 00 00 00       	mov    $0x8,%edi
  401a95:	e8 e6 1d 00 00       	callq  403880 <exit>

0000000000401a9a <read_six_numbers>:
  401a9a:	48 83 ec 18          	sub    $0x18,%rsp
  401a9e:	48 89 f2             	mov    %rsi,%rdx
  401aa1:	48 8d 76 14          	lea    0x14(%rsi),%rsi
  401aa5:	48 8d 42 10          	lea    0x10(%rdx),%rax
  401aa9:	48 8d 4a 04          	lea    0x4(%rdx),%rcx
  401aad:	48 89 74 24 08       	mov    %rsi,0x8(%rsp)
  401ab2:	48 89 04 24          	mov    %rax,(%rsp)
  401ab6:	4c 8d 4a 0c          	lea    0xc(%rdx),%r9
  401aba:	4c 8d 42 08          	lea    0x8(%rdx),%r8
  401abe:	be 92 5c 4a 00       	mov    $0x4a5c92,%esi
  401ac3:	b8 00 00 00 00       	mov    $0x0,%eax
  401ac8:	e8 33 2c 00 00       	callq  404700 <__isoc99_sscanf>
  401acd:	83 f8 05             	cmp    $0x5,%eax
  401ad0:	7f 05                	jg     401ad7 <read_six_numbers+0x3d>
  401ad2:	e8 8d ff ff ff       	callq  401a64 <explode_bomb>
  401ad7:	48 83 c4 18          	add    $0x18,%rsp
  401adb:	c3                   	retq   

0000000000401adc <read_line>:
  401adc:	48 83 ec 08          	sub    $0x8,%rsp
  401ae0:	b8 00 00 00 00       	mov    $0x0,%eax
  401ae5:	e8 1d fe ff ff       	callq  401907 <skip>
  401aea:	48 85 c0             	test   %rax,%rax
  401aed:	75 6e                	jne    401b5d <read_line+0x81>
  401aef:	48 8b 05 a2 41 2d 00 	mov    0x2d41a2(%rip),%rax        # 6d5c98 <_IO_stdin>
  401af6:	48 39 05 7b 7b 2d 00 	cmp    %rax,0x2d7b7b(%rip)        # 6d9678 <infile>
  401afd:	75 14                	jne    401b13 <read_line+0x37>
  401aff:	bf a4 5c 4a 00       	mov    $0x4a5ca4,%edi
  401b04:	e8 67 35 00 00       	callq  405070 <_IO_puts>
  401b09:	bf 08 00 00 00       	mov    $0x8,%edi
  401b0e:	e8 6d 1d 00 00       	callq  403880 <exit>
  401b13:	bf c2 5c 4a 00       	mov    $0x4a5cc2,%edi
  401b18:	e8 83 1b 00 00       	callq  4036a0 <getenv>
  401b1d:	48 85 c0             	test   %rax,%rax
  401b20:	74 0a                	je     401b2c <read_line+0x50>
  401b22:	bf 00 00 00 00       	mov    $0x0,%edi
  401b27:	e8 54 1d 00 00       	callq  403880 <exit>
  401b2c:	48 8b 05 65 41 2d 00 	mov    0x2d4165(%rip),%rax        # 6d5c98 <_IO_stdin>
  401b33:	48 89 05 3e 7b 2d 00 	mov    %rax,0x2d7b3e(%rip)        # 6d9678 <infile>
  401b3a:	b8 00 00 00 00       	mov    $0x0,%eax
  401b3f:	e8 c3 fd ff ff       	callq  401907 <skip>
  401b44:	48 85 c0             	test   %rax,%rax
  401b47:	75 14                	jne    401b5d <read_line+0x81>
  401b49:	bf a4 5c 4a 00       	mov    $0x4a5ca4,%edi
  401b4e:	e8 1d 35 00 00       	callq  405070 <_IO_puts>
  401b53:	bf 00 00 00 00       	mov    $0x0,%edi
  401b58:	e8 23 1d 00 00       	callq  403880 <exit>
  401b5d:	8b 15 0d 51 2d 00    	mov    0x2d510d(%rip),%edx        # 6d6c70 <num_input_strings>
  401b63:	48 63 c2             	movslq %edx,%rax
  401b66:	48 8d 34 80          	lea    (%rax,%rax,4),%rsi
  401b6a:	48 c1 e6 04          	shl    $0x4,%rsi
  401b6e:	48 81 c6 80 96 6d 00 	add    $0x6d9680,%rsi
  401b75:	48 89 f7             	mov    %rsi,%rdi
  401b78:	b8 00 00 00 00       	mov    $0x0,%eax
  401b7d:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
  401b84:	f2 ae                	repnz scas %es:(%rdi),%al
  401b86:	48 f7 d1             	not    %rcx
  401b89:	48 83 e9 01          	sub    $0x1,%rcx
  401b8d:	83 f9 4e             	cmp    $0x4e,%ecx
  401b90:	7e 46                	jle    401bd8 <read_line+0xfc>
  401b92:	bf cd 5c 4a 00       	mov    $0x4a5ccd,%edi
  401b97:	e8 d4 34 00 00       	callq  405070 <_IO_puts>
  401b9c:	8b 05 ce 50 2d 00    	mov    0x2d50ce(%rip),%eax        # 6d6c70 <num_input_strings>
  401ba2:	8d 50 01             	lea    0x1(%rax),%edx
  401ba5:	89 15 c5 50 2d 00    	mov    %edx,0x2d50c5(%rip)        # 6d6c70 <num_input_strings>
  401bab:	48 98                	cltq   
  401bad:	48 6b c0 50          	imul   $0x50,%rax,%rax
  401bb1:	48 bf 2a 2a 2a 74 72 	mov    $0x636e7572742a2a2a,%rdi
  401bb8:	75 6e 63 
  401bbb:	48 89 b8 80 96 6d 00 	mov    %rdi,0x6d9680(%rax)
  401bc2:	48 bf 61 74 65 64 2a 	mov    $0x2a2a2a64657461,%rdi
  401bc9:	2a 2a 00 
  401bcc:	48 89 b8 88 96 6d 00 	mov    %rdi,0x6d9688(%rax)
  401bd3:	e8 8c fe ff ff       	callq  401a64 <explode_bomb>
  401bd8:	83 e9 01             	sub    $0x1,%ecx
  401bdb:	48 63 c9             	movslq %ecx,%rcx
  401bde:	48 63 c2             	movslq %edx,%rax
  401be1:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
  401be5:	48 c1 e0 04          	shl    $0x4,%rax
  401be9:	c6 84 01 80 96 6d 00 	movb   $0x0,0x6d9680(%rcx,%rax,1)
  401bf0:	00 
  401bf1:	83 c2 01             	add    $0x1,%edx
  401bf4:	89 15 76 50 2d 00    	mov    %edx,0x2d5076(%rip)        # 6d6c70 <num_input_strings>
  401bfa:	48 89 f0             	mov    %rsi,%rax
  401bfd:	48 83 c4 08          	add    $0x8,%rsp
  401c01:	c3                   	retq   

0000000000401c02 <phase_defused>:
  401c02:	48 83 ec 78          	sub    $0x78,%rsp
  401c06:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401c0d:	00 00 
  401c0f:	48 89 44 24 68       	mov    %rax,0x68(%rsp)
  401c14:	31 c0                	xor    %eax,%eax
  401c16:	bf 01 00 00 00       	mov    $0x1,%edi
  401c1b:	e8 28 fd ff ff       	callq  401948 <send_msg>
  401c20:	83 3d 49 50 2d 00 06 	cmpl   $0x6,0x2d5049(%rip)        # 6d6c70 <num_input_strings>
  401c27:	75 6d                	jne    401c96 <phase_defused+0x94>
  401c29:	4c 8d 44 24 10       	lea    0x10(%rsp),%r8
  401c2e:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  401c33:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  401c38:	be e8 5c 4a 00       	mov    $0x4a5ce8,%esi
  401c3d:	bf 70 97 6d 00       	mov    $0x6d9770,%edi
  401c42:	b8 00 00 00 00       	mov    $0x0,%eax
  401c47:	e8 b4 2a 00 00       	callq  404700 <__isoc99_sscanf>
  401c4c:	83 f8 03             	cmp    $0x3,%eax
  401c4f:	75 31                	jne    401c82 <phase_defused+0x80>
  401c51:	be f1 5c 4a 00       	mov    $0x4a5cf1,%esi
  401c56:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  401c5b:	e8 30 fb ff ff       	callq  401790 <strings_not_equal>
  401c60:	85 c0                	test   %eax,%eax
  401c62:	75 1e                	jne    401c82 <phase_defused+0x80>
  401c64:	bf 20 5b 4a 00       	mov    $0x4a5b20,%edi
  401c69:	e8 02 34 00 00       	callq  405070 <_IO_puts>
  401c6e:	bf 48 5b 4a 00       	mov    $0x4a5b48,%edi
  401c73:	e8 f8 33 00 00       	callq  405070 <_IO_puts>
  401c78:	b8 00 00 00 00       	mov    $0x0,%eax
  401c7d:	e8 28 fa ff ff       	callq  4016aa <secret_phase>
  401c82:	bf 80 5b 4a 00       	mov    $0x4a5b80,%edi
  401c87:	e8 e4 33 00 00       	callq  405070 <_IO_puts>
  401c8c:	bf b0 5b 4a 00       	mov    $0x4a5bb0,%edi
  401c91:	e8 da 33 00 00       	callq  405070 <_IO_puts>
  401c96:	48 8b 44 24 68       	mov    0x68(%rsp),%rax
  401c9b:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  401ca2:	00 00 
  401ca4:	74 05                	je     401cab <phase_defused+0xa9>
  401ca6:	e8 35 af 02 00       	callq  42cbe0 <__stack_chk_fail>
  401cab:	48 83 c4 78          	add    $0x78,%rsp
  401caf:	90                   	nop
  401cb0:	c3                   	retq   
  401cb1:	90                   	nop
  401cb2:	90                   	nop
  401cb3:	90                   	nop
  401cb4:	90                   	nop
  401cb5:	90                   	nop
  401cb6:	90                   	nop
  401cb7:	90                   	nop
  401cb8:	90                   	nop
  401cb9:	90                   	nop
  401cba:	90                   	nop
  401cbb:	90                   	nop
  401cbc:	90                   	nop
  401cbd:	90                   	nop
  401cbe:	90                   	nop
  401cbf:	90                   	nop
